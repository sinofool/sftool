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

int main(void) {
	HttpUtil http;
	http.sendRequest("http://www.icbc.com.cn/");

	return EXIT_SUCCESS;
}
