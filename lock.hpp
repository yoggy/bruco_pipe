#ifndef ____LOCK__HPP____
#define ____LOCK__HPP____

#include <pthread.h>

class Mutex
{
	public:
		Mutex();
		virtual ~Mutex();

		int lock();
		int unlock();

	private:
		pthread_mutex_t mutex_;
};

class ScopedLock {
	public:
		ScopedLock(Mutex &mutex);
		virtual ~ScopedLock();

	private:
		Mutex *mutex_;
};


#endif // #define ____LOCK__HPP____
