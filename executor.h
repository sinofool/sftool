#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <boost/thread.hpp>

namespace sinofool {
	class Runnable {
	protected:
		virtual void run() = 0;
	};
	typedef boost::shared_ptr<Runnable> RunnablePtr;

	class Future {
	public:
		virtual int handle() = 0;
	};
	typedef boost::shared_ptr<Future> FuturePtr;

	class Executor {
	public:
		FuturePtr execute(const RunnablePtr& task);
	private:
		std::list<RunnablePtr> tasks;

	};
}

#endif