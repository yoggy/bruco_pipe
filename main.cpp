#include <string>
#include <iostream>
#include <stdlib.h>

#include <sstream>

#include "log.hpp"
#include "config.hpp"

#include "bruco_session.hpp"

void usage() 
{
	std::cerr << "usage : ./bruco_pipe config_file command [arg1] [arg2]..."  << std::endl;
	std::cerr << std::endl;
	std::cerr << "  examples" << std::endl;
	std::cerr << "    $ ./bruco_pipe sample_config.txt /path/to/program" << std::endl;
	std::cerr << std::endl;
	std::cerr << std::endl;

	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc < 3) usage();

	std::string config_file = argv[1];
	if (Config::load(config_file) == false) {
		log_e("config_file load failed....config_file=%s", config_file.c_str());
		return -1;
	}

	Config *cf = Config::getInstance();

	// nice setting
	if (cf->has_key("nice")) {
		int nice_val = cf->get_int("nice");
		nice(nice_val);
	}

	// logging settings
	set_output_log_level(cf->get_int("output_log_level"));
	set_output_string_length(cf->get_int("output_string_length"));

	// build command
	std::stringstream ss;
	for (int i = 2; i < argc; ++i) {
		ss << argv[i];
		ss << " ";
	}
	std::string cmd = ss.str();

	// session config
	RE2 *inbound_pass_re  = NULL;
	RE2 *inbound_deny_re  = NULL;
	RE2 *outbound_pass_re = NULL;
	RE2 *outbound_deny_re = NULL;

	std::string re;
	re = cf->get_string("outbound_pass");
	if (re != "") {
		outbound_pass_re = new RE2(re, RE2::Latin1);
	}

	re = cf->get_string("outbound_deny");
	if (re != "") {
		outbound_deny_re = new RE2(re, RE2::Latin1);
	}

	re = cf->get_string("inbound_pass");
	if (re != "") {
		inbound_pass_re = new RE2(re, RE2::Latin1);
	}

	re = cf->get_string("inbound_deny");
	if (re != "") {
		inbound_deny_re = new RE2(re, RE2::Latin1);
	}

	// create pipe
	BrucoSession session(argc, argv);

    session.outbound_key_check(cf->get_bool("outbound_key_check"));
    session.outbound_key_check_xor256(cf->get_bool("outbound_key_check_xor256"));
    session.outbound_key_file(cf->get_string("outbound_key_file"));

    session.inbound_jmpcall_check(cf->get_bool("inbound_jmpcall_check"));

    session.inbound_pass_re(inbound_pass_re);
    session.inbound_deny_re(inbound_deny_re);
    session.inbound_default_pass(cf->get_bool("inbound_default_pass"));
    session.outbound_pass_re(outbound_pass_re);
    session.outbound_deny_re(outbound_deny_re);
    session.outbound_default_pass(cf->get_bool("outbound_default_pass"));

    session.dump_stream(cf->get_bool("dump_stream"));
	
	session.start();

	return 0;
}

