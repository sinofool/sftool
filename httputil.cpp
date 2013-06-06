#include "StdAfx.h"

#include "httputil.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include "executor.h"

namespace sinofool {
void HttpUtil::sendRequest(const std::string& url) {
	HttpRequestPtr req = HttpRequestPtr(new HttpRequest(url));
	OutputTaskPtr task = OutputTaskPtr(new OutputTask(req));
	_exec.execute(task);
}

void HttpUtil::close() {
	_exec.shutdown();
	_exec.join();
}

void HttpRequest::doRequest() {
	CURL* curl = curl_easy_init();
	if (!curl) {
		return;
	}
	curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &HttpRequest::write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		printf("failed with: %s\n", curl_easy_strerror(res));
	}
	curl_easy_cleanup(curl);
}

size_t HttpRequest::callback(void* ptr, size_t size, size_t nmemb) {
	_html.append((const char*) ptr, size * nmemb);
	return size * nmemb;
}

size_t HttpRequest::write_data(void* ptr, size_t size, size_t nmemb,
		void* stream) {
	HttpRequest* hr = (HttpRequest*) stream;
	return hr->callback(ptr, size, nmemb);
}

void OutputTask::run() {
	_request->doRequest();
	std::cout << "HTML: " << _request->getResponse() << std::endl;
}
}
