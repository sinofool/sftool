#include "httputil.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include "executor.h"

namespace sinofool {
void http_util::get_request(const std::string& url,
		const std::map<std::string, std::string>& headers) {
	http_request_ptr req = http_request_ptr(
			new http_request(url, GET, headers, NULL));
	async_http_task_ptr task = async_http_task_ptr(new async_http_task(req));
	_exec.execute(task);
}

void http_util::post_request(const std::string& url,
		const std::map<std::string, std::string>& headers,
		const std::string& post_content) {
	http_request_ptr req = http_request_ptr(
			new http_request(url, POST, headers, post_content));
	async_http_task_ptr task = async_http_task_ptr(new async_http_task(req));
	_exec.execute(task);
}

void http_util::close() {
	_exec.shutdown();
	_exec.join();
}

void http_request::do_request() {
	CURL* curl = curl_easy_init();
	struct curl_slist *headers = NULL;

	if (!curl) {
		return;
	}
	curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
	for (std::list<std::string>::iterator it = _headers.begin();
			it != _headers.end(); ++it) {
		headers = curl_slist_append(headers, it->c_str());
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	if (_type == POST) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, _post_content.length());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, &http_request::read_data);
		curl_easy_setopt(curl, CURLOPT_READDATA, this);
	}
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &http_request::write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		printf("failed with: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
}

size_t http_request::callback(void* ptr, size_t size, size_t nmemb) {
	_html.append((const char*) ptr, size * nmemb);
	return size * nmemb;
}

size_t http_request::write_data(void* ptr, size_t size, size_t nmemb,
		void* stream) {
	http_request* hr = (http_request*) stream;
	return hr->callback(ptr, size, nmemb);
}

size_t http_request::read_data(void* ptr, size_t size, size_t nmemb,
		void* stream) {
	http_request* hr = (http_request*) stream;
	return hr->post_content(ptr, size, nmemb);
}

size_t http_request::post_content(void* ptr, size_t size, size_t nmemb) {
	size_t max_size = size * nmemb;
	size_t content_size = _post_content.length();
	size_t cpsize = max_size > content_size ? content_size : max_size;
	std::cout << "max: " << max_size << " content: " << content_size
			<< " cpsize: " << cpsize << std::endl;
	memcpy(ptr, _post_content.c_str(), cpsize);
	return cpsize;
}

void async_http_task::run() {
	_request->do_request();
	std::cout << "Async HTTP Task returns: " << _request->get_response()
			<< std::endl;
}
}
