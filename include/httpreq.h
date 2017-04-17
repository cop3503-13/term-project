#ifndef HTTPREQ_H
#define HTTPREQ_H

#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include "curl/curl.h"

class HTTPReq
{
public :

    HTTPReq();

    HTTPReq(const bool verbose);

    HTTPReq(const std::string url);

    HTTPReq(const std::string url,
            const bool verbose);

    HTTPReq(const std::string url,
            const bool verbose,
            const bool errOutput);

    ~HTTPReq();

    std::string getResponse();
    std::string getUrl();
    void setUrl(const std::string& url);
    void send();

    /* curl verbose output and
       errors sent to stderr */

    void setVerbose(const bool verbose);
    void setErrOutput(const bool errOutput);
    bool getVerbose();
    bool getErrOutput();

    bool isOk();
    const long getHTTPStatus();

    /* get timestamp for the last request
       or the last ok response */

    time_t getLastResponseTimestamp();
    time_t getLastSentTimestamp();

private :

    CURL *curl;                     // curl handler initialized by curl_easy_init
    CURLcode responseCode; 	        // curl return code

    std::string url;		        // url target for the request
    std::string response;           // response string from request

    char errbuf[CURL_ERROR_SIZE];   // buffer to store errors
    bool verbose;                   // verbose curl output
    bool errOutput;                 // write errors to stderr

    time_t lastSentTimestamp;       // when the last sent intent happened
    time_t lastResponseTimestamp;   // when the last ok response happened

    /* initializes and sets curl options */

    void init(std::string url = "",
              bool verbose = false,
              bool errOutput = true);

    /* callback function member provided to curl, writes
       the response in the response string field */

    static size_t writeResponse(void *contents, size_t size, size_t nmemb, void *userp);
};

#endif //HTTPREQ_H