#ifndef __BRUCO_PIPE_HPP__
#define __BRUCO_PIPE_HPP__

#include <unistd.h>
#include <string>

class BrucoPipe {
	public:
		BrucoPipe(int argc, char *argv[], const int &buf_size=1024);
		virtual ~BrucoPipe();

		void break_session();
		virtual bool start();

	protected:
		void process_stdio_loop_();

		virtual int send(const char *buf, int buf_size);
        virtual void on_recv(const char *buf, int buf_size);

        virtual int  send_proxy(const char *buf, int buf_size);
        virtual void on_recv_proxy(const char *buf, int buf_size);

	protected:
		char *path_;
		char **argv_;

		char *buf_;
		int  buf_size_;

		bool break_flag_;

		int pipe_out_;
		int pipe_in_;

		int inbound_count_;
		int outbound_count_;
		int inbound_total_bytes_;
		int outbound_total_bytes_;
};

#endif // #define __BRUCO_PIPE_HPP__
