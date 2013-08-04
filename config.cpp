#include "config.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "string.hpp"

Config * Config::singleton_ = NULL;
Mutex Config::singleton_mutex_;

Config * Config::getInstance()
{
	ScopedLock lock(singleton_mutex_);
	if (singleton_ == NULL) {
		singleton_ = new Config();
	}
	return singleton_;
}

Config::Config()
{
}

Config::~Config()
{
}

bool Config::has_key(const std::string &key)
{
	if (map_.find(key) == map_.end()) return false;
	return true;
}

bool Config::get_bool(const std::string &key)
{
	std::string val = get_string(key);
	if (val == "true" || val == "TRUE" || val == "on" || val == "ON") return true;
	return false;
}

int Config::get_int(const std::string &key)
{
	int val = atoi(get_string(key).c_str());
	return val;
}

std::string Config::get_string(const std::string &key)
{
	if (!has_key(key)) {
		return std::string();
	}
	return map_[key];
}

bool Config::load(const std::string &filename)
{
	Config *cf = getInstance();
	return cf->load_(filename);
}

bool Config::load_(const std::string &filename)
{
	ScopedLock lock(map_mutex_);

	map_.clear();

	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in);
	if (!ifs) {
		return false;
	}

	while(!ifs.eof()) {
		std::string l;
		getline(ifs, l);
		l = chomp(l);
		if (check_comment(l)) continue;			
		set_key_value_(l);
	}

	return true;
}

void Config::set_key_value_(const std::string &l)
{
	std::string key, val;
	if (split("=", l, key, val)) {
		map_[key] = val;
	}
}

