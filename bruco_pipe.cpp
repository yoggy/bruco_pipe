#include "bruco_pipe.hpp"
#include "log.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

BrucoPipe::BrucoPipe(int argc, char *argv[], const int &buf_size)
	: child_pid_(-1), buf_(NULL), buf_size_(buf_size), break_flag_(false), pipe_out_(-1), pipe_in_(-1),
	inbound_count_(0), outbound_count_(0), inbound_total_bytes_(0), outbound_total_bytes_(0)
{
	path_ = argv[2];
	int argv_size = argc - 2;
	if (argv_size < 0) {
		log_e("BrucoPipe::BrucoPipe() : argv size error...");
		return;
	}

	argv_ = (char**)malloc((argv_size + 1) * sizeof(char*));
	memset(argv_, 0, (argv_size + 1) * sizeof(char*));
	for (int i = 0; i < argv_size; ++i) {
		argv_[i] = argv[i + 2];
	}
	argv[argv_size] = NULL;
	
	buf_ = new char[buf_size];
}

BrucoPipe::~BrucoPipe()
{
	delete buf_;
	buf_ = NULL;

	free(argv_);
	argv_ = NULL;
}

void BrucoPipe::break_session()
{
	break_flag_ = true;
	kill(child_pid_, SIGKILL);
}

bool BrucoPipe::start()
{
	int pipefd[2];
	int rv = pipe(pipefd);
	if (rv < 0) {
		log_e("pipe() : failed...");
		return false;
	}

	pipe_in_  = pipefd[0];
	pipe_out_ = pipefd[1];

	child_pid_ = fork();

	if (child_pid_) {
		// child
		dup2(pipe_out_, fileno(stdout));
		dup2(pipe_in_,  fileno(stdin));

		close(pipe_out_);
		close(pipe_in_);

		execv(path_, argv_);
	}
	else {
		// parent
		process_stdio_loop_();
	}

	return true;
}

void BrucoPipe::process_stdio_loop_()
{
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(fileno(stdin), &fds);
	FD_SET(pipe_in_, &fds);
	int max_fd = std::max(fileno(stdin), pipe_in_) + 1;

	break_flag_ = false;

	while(!break_flag_) {
		fd_set work_fds = fds;
		int rv = select(max_fd, &work_fds, NULL, NULL, NULL);
		if (rv <= 0) break;

		// from client
		if (FD_ISSET(fileno(stdin), &work_fds) && break_flag_ == false) {
			memset(buf_, 0, buf_size_);
			int read_size = read(fileno(stdin), buf_, buf_size_);
			if (read_size <= 0) {
				break;
			}
			else {
				on_recv(buf_, read_size);
			}
		}

		// from server
		if (FD_ISSET(pipe_in_, &work_fds) && break_flag_ == false) {
			memset(buf_, 0, buf_size_);
			int read_size = read(pipe_in_, buf_, buf_size_);
			if (read_size <= 0) {
				break;
			}
			else {
				on_recv_proxy(buf_, read_size);
			}
		}
	}
}

int BrucoPipe::send(const char *buf, int buf_size)
{
    return ::write(fileno(stdout), buf, buf_size);
}

void BrucoPipe::on_recv(const char *buf, int buf_size)
{
    // log_d("on_recv() : size=%d, data=%s", buf_size, buf);

    // client -> proxy -> server
    send_proxy(buf, buf_size);

    inbound_count_ ++;
    inbound_total_bytes_ += buf_size;
}

int BrucoPipe::send_proxy(const char *buf, int buf_size)
{
    return ::write(pipe_out_, buf, buf_size);
}

void BrucoPipe::on_recv_proxy(const char *buf, int buf_size)
{
    //log_d("on_recv_proxy() : size=%d, data=%s", buf_size, buf);

    // server -> proxy -> client
    send(buf, buf_size);

    outbound_count_ ++;
    outbound_total_bytes_ += buf_size;
}

