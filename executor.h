#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <deque>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>

namespace sinofool {

template<typename T>
class executor {
public:
	typedef boost::shared_ptr<T> TaskPtr;
	typedef boost::shared_ptr<boost::thread> ThreadPtr;

	executor(int size) {
		_running = true;
		for (int i = 0; i < size; ++i) {
			_threads.push_back(
					ThreadPtr(new boost::thread(&executor::_exec, this)));
		}
	}
	void execute(const boost::shared_ptr<T>& task) {
		push(task);
	}
	void shutdown() {
		_running = false;
		_no_task.notify_all();
	}
	void join() {
		for (std::list<ThreadPtr>::iterator it = _threads.begin();
				it != _threads.end(); ++it) {
			(*it)->join();
		}
		std::cout << "Joined all threads." << std::endl;
	}
	static void _exec(executor* exec) {
		while (true) {
			TaskPtr task = exec->pull();
			if (task) {
				task->run();
				continue;
			}
			if (!exec->_running) {
				std::cout << "Finished running all task " << exec << std::endl;
				return;
			}
		};
	}
private:
	boost::atomic<bool> _running;
	std::deque<TaskPtr> _tasks;
	std::list<ThreadPtr> _threads;
	boost::mutex _mutex;
	boost::condition_variable _no_task;
	TaskPtr pull() {
		boost::unique_lock<boost::mutex> lock(_mutex);
		while (_running) {
			std::cout << "pull 0" << std::endl;
			if (!_tasks.empty()) {
				std::cout << "pull A" << std::endl;

				TaskPtr task = _tasks.front();
				_tasks.pop_front();
				return task;
			} else {
				std::cout << "pull B" << std::endl;
				_no_task.wait(lock);
				std::cout << "pull C" << std::endl;
			}
		}
		while (!_running) {
			std::cout << "pull 1" << std::endl;
			if (!_tasks.empty()) {
				std::cout << "pull D" << std::endl;
				TaskPtr task = _tasks.front();
				_tasks.pop_front();
				return task;
			} else {
				std::cout << "pull E" << std::endl;
				return TaskPtr();
			}
		}
		return TaskPtr();
	}

	void push(const TaskPtr& task) {
		if (!_running) {
			return;
		}
		boost::unique_lock<boost::mutex> lock(_mutex);
		std::cout << "push A" << std::endl;
		_tasks.push_back(task);
		if (_tasks.size() >= 1) {
			std::cout << "push B" << std::endl;
			_no_task.notify_one();
			std::cout << "push C" << std::endl;
		}
	}

};
}

#endif
