#include "StdAfx.h"

#include <iostream>
#include <set>
#include <boost/shared_ptr.hpp>
#include "executor.h"

#ifndef __HTTP_UTIL_H__
#define __HTTP_UTIL_H__

namespace sinofool {
class HttpRequest {
public:
	HttpRequest(const std::string& url) :
			_url(url) {
	}
	void doRequest();
	std::string getResponse() {
		return _html;
	}
private:
	static size_t write_data(void* ptr, size_t size, size_t nmemb,
			void* stream);
	size_t callback(void* ptr, size_t size, size_t nmemb);
	std::string _url;
	std::string _html;
};
typedef boost::shared_ptr<HttpRequest> HttpRequestPtr;

class OutputTask {
public:
	OutputTask(HttpRequestPtr request) :
			_request(request) {
	}
	void run();
private:
	HttpRequestPtr _request;
};
typedef boost::shared_ptr<OutputTask> OutputTaskPtr;

class HttpUtil {
public:
	HttpUtil(int size) :
			_exec(size) {
	}
	void sendRequest(const std::string& url);
	void close();
private:
	Executor<OutputTask> _exec;
};

}
#endif
