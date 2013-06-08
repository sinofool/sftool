#include <iostream>
#include <set>
#include <boost/shared_ptr.hpp>
#include "executor.h"

#ifndef __HTTP_UTIL_H__
#define __HTTP_UTIL_H__

namespace sinofool {
enum HTTP_REQUEST_TYPE {
	GET, POST
};

class http_request {
public:
	http_request(const std::string& url, HTTP_REQUEST_TYPE type,
			const std::map<std::string, std::string>& headers,
			const std::string& post_content) :
			_url(url), _type(type), _post_content(post_content) {
		for (std::map<std::string, std::string>::const_iterator it =
				headers.begin(); it != headers.end(); ++it) {
			_headers.push_back(it->first + ": " + it->second);
		}
	}
	void do_request();
	std::string get_response() {
		return _html;
	}
private:
	static size_t write_data(void* ptr, size_t size, size_t nmemb,
			void* stream);
	static size_t read_data(void* ptr, size_t size, size_t nmemb,
				void* stream);
	size_t callback(void* ptr, size_t size, size_t nmemb);
	size_t post_content(void* ptr, size_t size, size_t nmemb);
	std::string _url;
	HTTP_REQUEST_TYPE _type;
	std::list<std::string> _headers;
	std::string _post_content;

	std::string _html;

};
typedef boost::shared_ptr<http_request> http_request_ptr;

class async_http_task {
public:
	async_http_task(http_request_ptr request) :
			_request(request) {
	}
	void run();
private:
	http_request_ptr _request;
};
typedef boost::shared_ptr<async_http_task> async_http_task_ptr;

class http_util {
public:
	http_util(int size) :
			_exec(size) {
	}
	void get_request(const std::string& url,
			const std::map<std::string, std::string>& headers);
	void post_request(const std::string& url,
			const std::map<std::string, std::string>& headers,
			const std::string& post_content);
	void close();
private:
	executor<async_http_task> _exec;
};
typedef boost::shared_ptr<http_util> http_util_ptr;

}
#endif
