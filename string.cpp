#include "string.hpp"
#include <stdio.h>
#include <sstream>

std::string chomp(const std::string &str)
{
	std::stringstream buf;

	for (unsigned int i = 0; i < str.size(); ++i) {
		if (str[i] == 0x0a || str[i] == 0x0d) continue;
		buf << str[i];
	}

	return buf.str(); 
}

bool check_comment(const std::string &l)
{
	if (l.size() == 0) return true;
	if (l[0] == '#')   return true;
	if (l.find_first_of("=") == std::string::npos) return true;

	return false;
}

std::string strip(const std::string &str)
{
	int pos = 0;
	std::string tmp, result;

	if (str.size() == 0) return "";

	// remove whitespace from the head of string.
	for (pos = 0; pos < (int)str.size(); ++pos) {
		if (!(str[pos] == ' ' || str[pos] == '\t')) break;
	}

	if (pos < (int)str.size()) {
		tmp = str.substr(pos);
	}
	else {
		return "";
	}

	// remove whitespace from the end of string.
	for (pos = (int)str.size() - 1; pos > 0; --pos) {
		if (!(str[pos] == ' ' || str[pos] == '\t')) break;
	}

	if (pos >= 0) {
		result = tmp.substr(0, pos+1);
	}
	else {
		return "";
	}

	return result; 
}

bool split(const std::string &separator, const std::string &src, std::string &key, std::string &val)
{
	size_t pos = src.find_first_of(separator);
	if (pos == std::string::npos) {
		return false;
	}

	key = src.substr(0, pos-1);
	val = src.substr(pos+1);

	key = strip(key);
	val = strip(val);

	if (key.size() == 0 || val.size() == 0) {
		return false;
	}

	return true;
}

std::string escape(const unsigned char c)
{
	char buf[5];
	int n = c;
	snprintf(buf, 5, "\\x%02x", n);
	return std::string(buf);
}

std::string escape(const std::string &str)
{
	std::stringstream res;

	for (int i = 0; i < (int)str.size(); ++i) {
		unsigned char c = str[i];
		if (isprint(c)) {
			res << str[i];
		}
		else {
			res << escape(c);
		}
	}

	return res.str();
}

std::string xor8(const std::string &str, const unsigned char x)
{
	std::stringstream res;

	for (int i = 0; i < (int)str.size(); ++i) {
		unsigned char c = str[i];
		res << (unsigned char)((c ^ x) & 0xff);
	}
	
	return res.str();
}
