bruco_pipe
================
bruco_pipe is simple intrusion prevention pipe filter.


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
for Linux, FreeBSD
<pre>
$ git clone https://github.com/yoggy/bruco_pipe.git
$ cd bruco_pipe
$ make
</pre>

for Mac OS X
<pre>
$ git clone https://github.com/yoggy/bruco_pipe.git
$ cd bruco_pipe
$ make CPPFLAGS=-DMAC_OS_X
</pre>

run
================
see the "make test" for more information...
<pre>
usage : ./bruco_pipe config_file command [arg1] [arg2]...

  examples
    $ ./bruco_pipe sample_config.txt /path/to/program

$ ./bruco_pipe sample_config.txt /bin/date
2013-08-03T19:59:12+0900 [D] dump_stream : [out_bound] Sat Aug  3 19:59:12 JST 2013\x0a  &lt;- stderr

$ ./bruco_pipe sample_config.txt ./test_server
2013-08-03T18:56:57+0900 [D] dump_stream : [out_bound] test0\x0a   &lt;- stderr
test0                                                              &lt;- stdout
2013-08-03T18:56:58+0900 [D] dump_stream : [out_bound] test1\x0a
test1
2013-08-03T18:56:59+0900 [D] dump_stream : [out_bound] test2\x0a
test2
2013-08-03T18:57:00+0900 [D] dump_stream : [out_bound] test3\x0a
test3
2013-08-03T18:57:01+0900 [D] dump_stream : [out_bound] test4\x0a
test4
2013-08-03T18:57:02+0900 [D] dump_stream : [out_bound] bbbb\x0a
2013-08-03T18:57:02+0900 [W] break_session : type=outbound_deny_re, outbound_re=\x62\x62\x62\x62  &lt;- pipe disconnect
</pre>

config file
================
<pre>
# log level (0:debug, 1:info, 2:warn)
output_log_level = 0

# max output string length
output_string_length = 1024

# udp log output
udp_log_output_enable = false
udp_log_output_host   = 127.0.0.1
udp_log_output_port   = 12345

# dump stream (true / false)
dump_stream = true

# nice() setting
nice = 5

# regular expression (deny)
inbound_deny  = \x62\x62\x62\x62
outbound_deny = \x62\x62\x62\x62

# regular expression (pass)
#inbound_pass  = \x62\x62\x62\x62
#outbound_pass = \x62\x62\x62\x62

# default policy (true / false)
inbound_default_pass  = true
outbound_default_pass = true
</pre>

