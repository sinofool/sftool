#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

namespace sinofool {

	template <typename T>
	class Executor {
	public:
		Executor() {
			_threads.push_back(ThreadPtr(new boost::thread(&Executor::_exec, this)));
		}
		void execute(const T& task) {
			task->run();
		}
		static void _exec(Executor* exec) {
			std::cout << "I am running" << std::endl;
		}
	private:
		std::list<T> _tasks;
		typedef boost::shared_ptr<boost::thread> ThreadPtr;
		std::list<ThreadPtr> _threads;

	};
}

#endif
