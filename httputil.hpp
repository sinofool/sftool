#include <iostream>
#include <set>
#include <boost/shared_ptr.hpp>
#include "executor.hpp"
#include "http_request.h"

#ifndef __HTTP_UTIL_H__
#define __HTTP_UTIL_H__

SFTOOL_NAMESPACE_START

template<typename T>
class async_http_task {
public:
    async_http_task(http_request_ptr request, const boost::shared_ptr<T>& response) :
    _request(request), _response(response) {
    }
    void run() {
        _request->do_request();
        std::cout << "Async HTTP Task returns: \n" << _request->get_response()
        << std::endl;
        _response->handle(_request->get_response());
    };
private:
    http_request_ptr _request;
    boost::shared_ptr<T> _response;
};

template<typename T>
class http_util {
public:
    http_util(int size) :
    _exec(size) {
    }
    boost::shared_ptr<T> get_request(const std::string& url,
                                     const std::map<std::string, std::string>& headers) {
        http_request_ptr req = http_request_ptr(
                                                new http_request(url, GET, headers, ""));
        boost::shared_ptr<T> res = T::create();
        boost::shared_ptr<async_http_task<T> > task = boost::shared_ptr<async_http_task<T> >(new async_http_task<T>(req, res));
        _exec.execute(task);
        return res;
    }
    boost::shared_ptr<T> post_request(const std::string& url,
                                      const std::map<std::string, std::string>& headers,
                                      const std::string& post_content) {
        http_request_ptr req = http_request_ptr(
                                                new http_request(url, POST, headers, post_content));
        boost::shared_ptr<T> res = T::create();
        boost::shared_ptr<async_http_task<T> > task = boost::shared_ptr<async_http_task<T> >(new async_http_task<T>(req, res));
        _exec.execute(task);
        return res;
    };
    void close() {
        _exec.shutdown();
        _exec.join();
    };
private:
    executor<async_http_task<T> > _exec;
};


SFTOOL_NAMESPACE_END

#endif

