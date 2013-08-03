#ifndef ____STRING__HPP____
#define ____STRING__HPP____

#include <string>

std::string chomp(const std::string &str);
bool check_comment(const std::string &l);
std::string strip(const std::string &str);
bool split(const std::string &separator, const std::string &src, std::string &key, std::string &val);

std::string escape(const std::string &str);
std::string xor8(const std::string &str, const unsigned char x);

#endif // #define ____STRING__HPP____
