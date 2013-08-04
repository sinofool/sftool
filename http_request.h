//
//  http_request.h
//  coins
//
//  Created by Bai Bochun on 8/5/13.
//
//

#ifndef __http_request_h__
#define __http_request_h__

#include "sftool.h"
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>

SFTOOL_NAMESPACE_START

enum HTTP_REQUEST_TYPE {
    GET, POST
};

class http_request {
public:
    http_request(const std::string& url, HTTP_REQUEST_TYPE type,
                 const std::map<std::string, std::string>& headers,
                 const std::string& post_content) :
    _url(url), _type(type), _post_content(post_content), _post_pos(0) {
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
    size_t _post_pos;
    std::string _html;
    
};
typedef boost::shared_ptr<http_request> http_request_ptr;

class string_http_handler {
public:
    static boost::shared_ptr<string_http_handler> create();
    void handle(const std::string& response);
};


SFTOOL_NAMESPACE_END

#endif
