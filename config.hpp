#ifndef ____CONFIG__HPP____
#define ____CONFIG__HPP____

#include <string>
#include <map>
#include "lock.hpp"

class Config
{
	private:
		Config();
		virtual ~Config();

	public:
		bool get_bool(const std::string &key);
		int  get_int(const std::string &key);
		std::string get_string(const std::string &key);

	public:
		static Config *getInstance();
		static bool load(const std::string &filename);

	private:
		void set_key_value_(const std::string &l);
		bool load_(const std::string &filename);

	private:
		std::map<std::string, std::string> map_;
		Mutex map_mutex_;
		
	private:
		static Config *singleton_;
		static Mutex   singleton_mutex_;
};



#endif // #define ____CONFIG__HPP____
