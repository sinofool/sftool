
#include <iostream>
#include <set>
#include <boost/shared_ptr.hpp>
#include "executor.h"

#ifndef __HTTP_UTIL_H__
#define __HTTP_UTIL_H__

namespace sinofool {
	class HttpRequest {
	public:
		HttpRequest(const std::string& url) : _url(url){};
		FuturePtr sendRequest();
	private:
		static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);
		size_t callback(void* ptr, size_t size, size_t nmemb);
		std::string _url;
		std::string _html;
	};

	class HttpUtil {
	public:
		void sendRequest(const std::string& url);
	private:
		std::set<HttpRequest*> pending;
	};

	class OutputFuture : virtual public Future {
	public:
		OutputFuture(HttpRequest* request) : _request(request) {};
	protected:
		virtual int handle();
	private:
		HttpRequest* _request;
	};

}
#endif
