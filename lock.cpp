#include "lock.hpp"

Mutex::Mutex()
{
	pthread_mutex_init(&mutex_, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex_);
}

int Mutex::lock()
{
	return pthread_mutex_lock(&mutex_);
}

int Mutex::unlock()
{
	return pthread_mutex_unlock(&mutex_);
}

ScopedLock::ScopedLock(Mutex &mutex)
{
	this->mutex_ = &mutex;
	this->mutex_->lock();
}

ScopedLock::~ScopedLock()
{
	this->mutex_->unlock();
	this->mutex_ = NULL;
}
