#ifndef __HTTP_REQUEST_H
#define __HTTP_REQUEST_H

#include <curl/curl.h> 
#include <string>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 

using std::string;
using std::cout;
using std::cin;
using std::endl;


namespace ns_http_helper {

struct http_handler{
    CURL *curl;
    char *buf;
    int size;
    int len;
    int error;
    struct curl_slist *header;
} ;

void http_global_init();
void http_global_cleanup();

size_t http_write_function(void *buffer, size_t size, size_t nmemb, void *userp);

http_handler *http_init(int size) ;

void http_cleanup(http_handler *http);

void http_request_init(http_handler *http);

int http_request(http_handler *http, const char *url, const std::string &post_data,  int to = 0, int cto = 0, const char *msg="");

int http_post_request(http_handler* http, std::string& uri, std::string& request, int to, int cto, std::string& ret_data);

}; // namespace ns_http_helper

#endif // __HTTP_REQUEST_H

