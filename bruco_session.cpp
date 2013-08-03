#include "bruco_session.hpp"

#include <fstream>
#include <sstream>

#include "log.hpp"
#include "string.hpp"

BrucoSession::BrucoSession(int argc, char *argv[], const int &buf_size)
	: BrucoPipe(argc, argv, buf_size),
	outbound_key_check_(false), outbound_key_check_xor256_(false), 
	inbound_jmpcall_check_(false), 
	inbound_pass_re_(NULL), inbound_deny_re_(NULL), 
	outbound_pass_re_(NULL), outbound_deny_re_(NULL),
	inbound_default_pass_(false), outbound_default_pass_(false),
	dump_stream_(false)
{

}

BrucoSession::~BrucoSession()
{

}

void BrucoSession::outbound_key_check(const bool &flag)
{
	outbound_key_check_ = flag;
}

void BrucoSession::outbound_key_check_xor256(const bool &flag)
{
	outbound_key_check_xor256_ = flag;
}

void BrucoSession::outbound_key_file(const std::string &val)
{
	outbound_key_file_ = val;
}

void BrucoSession::inbound_jmpcall_check(const bool &flag)
{
	inbound_jmpcall_check_ = flag;
}

void BrucoSession::outbound_pass_re(RE2 *re)
{
	outbound_pass_re_ = re;
}

void BrucoSession::outbound_deny_re(RE2 *re)
{
	outbound_deny_re_ = re;
}

void BrucoSession::outbound_default_pass(const bool &flag)
{
	this->outbound_default_pass_ = flag;
}

void BrucoSession::inbound_pass_re(RE2 *re)
{
	inbound_pass_re_ = re;
}

void BrucoSession::inbound_deny_re(RE2 *re)
{
	inbound_deny_re_ = re;
}

void BrucoSession::inbound_default_pass(const bool &flag)
{
	this->inbound_default_pass_ = flag;
}

void BrucoSession::dump_stream(const bool &flag)
{
	dump_stream_ = flag;
}

void BrucoSession::on_recv(const char *buf, int buf_size)
{
	std::string target_str(buf, buf_size);

	if (dump_stream_) {
		log_d("dump_stream : [in_bound] %s", escape(target_str).c_str());
	}

	if (inbound_jmpcall_check_) {
		bool rv = check_jmpcall_(target_str);
		if (rv) {
			log_w("break_session : type=inbound_jmpcall");
			break_session();
			return;
		}
	}

	if (inbound_pass_re_ != NULL) {
		bool rv = RE2::PartialMatch(target_str, *inbound_pass_re_);
		if (rv) {
			BrucoPipe::on_recv(buf, buf_size);
			return;
		}
	}

	if (inbound_deny_re_ != NULL) {
		bool rv = RE2::PartialMatch(target_str, *inbound_deny_re_);
		if (rv) {
			log_w("break_session : type=inbound_deny_re, inbound_re=%s", inbound_deny_re_->pattern().c_str());
			break_session();
			return;
		}
	}

	// default policy 
	if (inbound_default_pass_) {
		BrucoPipe::on_recv(buf, buf_size);
	}
	else {
		break_session();
	}
}

void BrucoSession::on_recv_proxy(const char *buf, int buf_size)
{
	std::string target_str(buf, buf_size);
	if (dump_stream_) {
		log_d("dump_stream : [out_bound] %s", escape(target_str).c_str());
	}

	if (outbound_key_check_xor256_) {
		std::string key = read_key_();
		if (check_contain_key_xor256_(key, target_str)) {
			log_w("break_session : type=outbound_contain_key_xor256, outbound_key_file=%s",
				outbound_key_file_.c_str());
			break_session();
			return;
		}
	}
	else if (outbound_key_check_) {
		std::string key = read_key_();
		if (check_contain_key_(key, target_str)) {
			log_w("break_session : %s, type=outbound_contain_key, outbound_key_file=%s",
				outbound_key_file_.c_str());
			break_session();
			return;
		}
	}

	if (outbound_pass_re_ != NULL) {
		bool rv = RE2::PartialMatch(target_str, *outbound_pass_re_);
		if (rv) {
			BrucoPipe::on_recv_proxy(buf, buf_size);
			return;
		}
	}

	if (outbound_deny_re_ != NULL) {
		bool rv = RE2::PartialMatch(target_str, *outbound_deny_re_);
		if (rv) {
			log_w("break_session : type=outbound_deny_re, outbound_re=%s",
				outbound_deny_re_->pattern().c_str());
			break_session();
			return;
		}
	}

	// default policy
	if (outbound_default_pass_) {
		BrucoPipe::on_recv_proxy(buf, buf_size);
	}
	else {
		break_session();
	}
}

std::string BrucoSession::read_key_()
{
	std::ifstream ifs;
	std::stringstream ss;

	ifs.open(outbound_key_file_.c_str(), std::ios::in);
	if (!ifs) {
		log_w("outbound_key_file load failed...outbound_key_file=%s", outbound_key_file_.c_str());
		return "";
	}

	ss << ifs.rdbuf();
	ifs.close();
	
	return ss.str();
}

bool BrucoSession::check_contain_key_(const std::string &key, const std::string &src)
{
	if (key.size() == 0) return false;
	if (src.size() == 0) return false;

	RE2 re(RE2::QuoteMeta(key), RE2::Latin1);

	return RE2::PartialMatch(src, re);
}

bool BrucoSession::check_contain_key_xor256_(const std::string &key, const std::string &src)
{
	if (key.size() == 0) return false;
	if (src.size() == 0) return false;

	for (int n = 0; n < 256; ++n) {
		std::string xor_str = xor8(key, (unsigned char)n);

		RE2 re(RE2::QuoteMeta(xor_str), RE2::Latin1);
		if (RE2::PartialMatch(src, re)) return true;
	}

	return false;
}

bool BrucoSession::check_jmpcall_(const std::string &src)
{
	if (src.size() == 0) return false;

	for (int p = 0; p < (int)src.size(); ++p) {
		// jmp check
		if (src[p] == (char)0xef) { 
			if (p + 1 >= (int)src.size()) continue;
			char jmpoffset = src[p+1];
			
			int callpos = p + jmpoffset + 2;
			if (callpos < 0 || callpos >= (int)src.size()) continue;
			
			// call check
			if (src[callpos] == (char)0xe8) {
				return true;
			}
		}
	}

	return false;
}

