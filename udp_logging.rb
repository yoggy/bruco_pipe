#!/usr/bin/ruby
require "socket"

def usage
	puts "usage : ./udp_logging.rb udp_port"
	puts 
	puts "    example : "
	puts "        $ ./udp_logging.rb 58234"
	puts 
	exit(1)

end

usage if ARGV.size != 1

udp = UDPSocket.open()
udp.bind("0.0.0.0", ARGV[0].to_i)

loop do
	print udp.recv(65535)
	$stdout.flush
end
