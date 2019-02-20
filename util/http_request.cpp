
#include "http_request.h"

namespace http_helper{

//#define __F_TESING__

void http_global_init() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void http_global_cleanup() {
    curl_global_cleanup();
}

size_t http_write_function(void *buffer, size_t size, size_t nmemb, void *userp) 
{ 
    http_handler *http = (http_handler *)userp;
    int bufsize = size * nmemb; 
    if (http->len + bufsize >= http->size) { 
        http->error = 1; 
        return 0; 
    } 
    memcpy(http->buf + http->len, buffer, bufsize); 
    http->len += bufsize; 
    http->buf[http->len] = 0; 
    return bufsize; 
} 

http_handler *http_init(int size) 
{ 
    CURL *curl = curl_easy_init(); 
    if (!curl) { 
        cout << "failed to init curl\n";
        return NULL; 
    } 
    http_handler *http = (http_handler *)malloc(sizeof(http_handler));
    memset(http, 0, sizeof(http_handler));
    http->curl = curl;
    http->buf = (char *)malloc(size);
    http->size = size;
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)http);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_function);
    //curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    //证书相关
    std::string cafile = "/etc/pki/tls/certs/ca-bundle.crt";
    //curl_easy_setopt(curl, CURLOPT_CAINFO, cafile.c_str());
    //curl_easy_setopt(curl, CURLOPT_SSLCERT, cafile.c_str());
    //curl_easy_setopt(curl, CURLOPT_SSLKEY, cafile.c_str());
    //curl_easy_setopt(curl, CURLOPT_KEYPASSWD, "s3cret");
    struct curl_slist * headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:application/json;charset=\'utf-8\'");
    headers = curl_slist_append(headers, "Accept:application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    http->header = headers;

    return http;
}

void http_cleanup(http_handler *http)
{
    if (http) curl_easy_cleanup(http->curl); 
    if (http->buf) free(http->buf);
    http->curl = NULL;
    http->buf = NULL;
}

void http_request_init(http_handler *http)
{
    http->len = 0;
    http->error = 0;
}

int http_request(http_handler *http, const char *url, const std::string &post_data, int to, int cto, const char *msg)
{
    CURL *curl = http->curl;
    curl_easy_setopt(curl, CURLOPT_URL, url); 
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, to ? to : 200); 
    if (cto > 0) curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, cto); 
    //curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1); 
	curl_easy_setopt(curl, CURLOPT_POST, true);
    if (!post_data.empty()){
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.data()); 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)post_data.size()); 
    } else {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ""); 
    }
    http->len = 0;
    http->error = 0;
    CURLcode ret = curl_easy_perform(curl); 
    //curl_slist_free_all(http->header);
    if (ret != CURLE_OK) {
        cout << "[ " << msg << " ] [error]"
            << " perform: " << ret 
            << " url: " << url
            << " to: " << to
            << " cto: " << cto;
        http->error = ret;
        return ret;
    }

#ifdef __F_TESING__
    double name_tm  = 0.0; 
    double conn_tm  = 0.0;
    double aconn_tm = 0.0; 
    double pret_tm  = 0.0;
    double rcv_tm   = 0.0; 
    double total_tm = 0.0;
    int curlRC = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME,    &name_tm);
    curlRC     = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME,       &conn_tm);
    curlRC     = curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME,    &aconn_tm);
    curlRC     = curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME,   &pret_tm);
    curlRC     = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &rcv_tm);
    curlRC     = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME,         &total_tm);
    char temp[1024] = {0};
    snprintf(temp, sizeof(temp), "%s: URL:%s TO:%d CTO:%d \
            Time INFO name_tm:%lf conn_tm:%lf aconn_tm:%lf pret_tm:%lf rcv_tm:%lf total_tm:%lf", 
            __func__, url, to, cto,
            name_tm, conn_tm, aconn_tm, pret_tm, rcv_tm, total_tm);
    cout << temp;
#endif

    long retcode = 0;
    ret = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode); 
    if (ret != CURLE_OK) {
        cout << "[ " << msg << " ] [error] getinfo: " << ret << " url: " << url;
        http->error = ret;
        return -1;
    }
    if (retcode != 200 ) {
        cout << "[ " << msg << " ] [error] retcode: " << retcode << " url: " << url;
        http->error = retcode;
        return -2;
    }

    return 0; 
}

int http_post_request(http_handler* http, std::string& uri, std::string& request, int to, int cto, std::string& ret_data) {
    if (http == NULL || uri.empty()) {
        return -1;
    }
    http_request_init(http);
    int status = http_request(http, uri.c_str(), request, to, cto, __func__);
    cout << "http_post_request, uri=" << uri
        << " post=" << request
        << " to=" << to
        << " cto=" << cto;

    if (status != 0 || http->len <= 0) {
        char temp[1024] = { 0 };
        snprintf(temp, sizeof(temp), "%s: get response faild! http[%p], ec[%d][%d], body[%p][%d]", 
            __func__, http, status, http->error, http->buf, http->len);
        cout << temp;
        return -1;
    } else {
        ret_data = std::string(http->buf, http->len);
        return 0;
    }
}

}; // namespace http_helper

