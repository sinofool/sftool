#include "StdAfx.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>

namespace sinofool {
	void HttpUtil::sendRequest(const std::string& url) {
		HttpRequest req(url);
		// TODO make it background.
		FuturePtr future = req.sendRequest();
		future->handle();
	}

	FuturePtr HttpRequest::sendRequest() {
		CURL* curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "http://www.icbc.com.cn/");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &HttpRequest::write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

			CURLcode res = curl_easy_perform(curl);
 			if (res != CURLE_OK) {
				printf("failed with: %s\n", curl_easy_strerror(res));
			}
			curl_easy_cleanup(curl);
		}

		FuturePtr future = FuturePtr(new OutputFuture(this));
		return future;
	}

	size_t HttpRequest::callback(void* ptr, size_t size, size_t nmemb) {
		_html.append((const char*)ptr, size*nmemb);
		return size*nmemb;
	};

	size_t HttpRequest::write_data( void* ptr, size_t size, size_t nmemb, void* stream )
	{
		HttpRequest* hr = (HttpRequest*) stream;
		return hr->callback(ptr, size, nmemb);
	}

	int OutputFuture::handle()
	{
		std::cout << "HTML: " << _request << std::endl;
		return 1;
	}

};