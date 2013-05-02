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
	HttpUtil http(10);
	http.sendRequest("http://www.ea.com/");
	http.sendRequest("http://www.ea.com/");
	sleep(1);
	http.sendRequest("http://www.ea.com/");
	http.close();

	return EXIT_SUCCESS;
}
