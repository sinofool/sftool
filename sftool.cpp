//============================================================================
// Name        : sftool.cpp
// Author      : Bai Bochun
// Version     :
// Copyright   : Copyright SINOFOOL @2013
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "httputil.h"

using namespace sinofool;

int main() {
	http_util http(10);
	std::map<std::string, std::string> headers;
	std::string content = "HelloPost=1";
	http.post_request("http://10.88.73.52/dump.php", headers, content);
	http.post_request("http://10.88.73.52/dump.php", headers, content);
	sleep(1);
	http.post_request("http://10.88.73.52/dump.php", headers, content);
	http.close();

	return EXIT_SUCCESS;
}
