#ifndef __BRUCO_SESSION_HPP__
#define __BRUCO_SESSION_HPP__

#include <re2/re2.h>
#include "bruco_pipe.hpp"
#include "lock.hpp"

class BrucoSession : public BrucoPipe
{
	public:
		BrucoSession(int argc, char *argv[], const int &buf_size=1024);
		virtual ~BrucoSession();

		void outbound_key_check(const bool &flag);
		void outbound_key_check_xor256(const bool &flag);
		void outbound_key_file(const std::string &val);

		void outbound_pass_re(RE2 *re);
		void outbound_deny_re(RE2 *re);
		void outbound_default_pass(const bool &flag);

		void inbound_pass_re(RE2 *re);
		void inbound_deny_re(RE2 *re);
		void inbound_default_pass(const bool &flag);

		void inbound_jmpcall_check(const bool &flag);

		void dump_stream(const bool &flag);

		virtual void on_recv(const char *buf, int buf_size);
		virtual void on_recv_proxy(const char *buf, int buf_size);

	protected:
		std::string read_key_();
		bool check_contain_key_(const std::string &key, const std::string &src);
		bool check_contain_key_xor256_(const std::string &key, const std::string &src);
		bool check_jmpcall_(const std::string &key);

	protected:
		long session_id_;
		bool outbound_key_check_;
		bool outbound_key_check_xor256_;
		std::string outbound_key_file_;

		bool inbound_jmpcall_check_;

		RE2* inbound_pass_re_;
		RE2* inbound_deny_re_;
		RE2* outbound_pass_re_;
		RE2* outbound_deny_re_;

		bool inbound_default_pass_;
		bool outbound_default_pass_;

		bool dump_stream_;

	protected:
		static long get_session_id();
		static Mutex session_id_mutex_;
};

#endif // #define __BRUCO_SESSION_HPP__
