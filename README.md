bruco_pipe
================
bruco_pipe is simple intrusion prevention proxy.


requirements
================
<pre>
  $ wget http://re2.googlecode.com/files/re2-20130622.tgz
  $ tar xvfz re2-20130622.tgz
  $ cd re2
  $ make
  $ make install
</pre>


build
================
<pre>
$ git clone https://github.com/yoggy/bruco_pipe.git
$ cd bruco_pipe
$ cmake .
$ make
</pre>

run
================
<pre>
usage : ./bruco_pipe config_file command [arg1] [arg2]...

  examples
    $ ./bruco_pipe sample_config.txt /path/to/program

$ ./bruco_pipe sample_config.txt /bin/date
2013-08-01T11:25:37+0900 [I] listen start...port=12345

$ ./bruco_pipe sample_config.txt ./test_server
2013-08-03T18:56:57+0900 [D] dump_stream : [out_bound] test0\x0a
test0
2013-08-03T18:56:58+0900 [D] dump_stream : [out_bound] test1\x0a
test1
2013-08-03T18:56:59+0900 [D] dump_stream : [out_bound] test2\x0a
test2
2013-08-03T18:57:00+0900 [D] dump_stream : [out_bound] test3\x0a
test3
2013-08-03T18:57:01+0900 [D] dump_stream : [out_bound] test4\x0a
test4
2013-08-03T18:57:02+0900 [D] dump_stream : [out_bound] bbbb\x0a
2013-08-03T18:57:02+0900 [W] break_session : type=outbound_deny_re, outbound_re=\x62\x62\x62\x62
</pre>

config file
================
<pre>
# log level (0:debug, 1:info, 2:warn)
output_log_level = 0

# max output string length
output_string_length = 1024

# network config
listen_port = 12345
forward_host  = ::1
forward_port  = 23456 
max_client  = 100

# dump stream (true / false)
dump_stream = false

# regular expression check
inbound_deny  = test\x61\x61\x61\x61
outbound_deny = test\x62\x62\x62\x62

</pre>
