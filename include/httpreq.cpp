#include "httpreq.h"
#include "curl/curl.h"

/* TODO:
    Don't construct object if curl null (exception)
*/

HTTPReq::HTTPReq()
{
    init();
}

HTTPReq::HTTPReq(const bool verbose)
{
    init("", verbose);
}

HTTPReq::HTTPReq(const std::string url)
{
    init(url);
}

HTTPReq::HTTPReq(const std::string url, const bool verbose)
{
    init(url, verbose);
}

HTTPReq::HTTPReq(const std::string url,
                 const bool verbose,
                 const bool errOutput)
{
    init(url, verbose, errOutput);
}

HTTPReq::~HTTPReq()
{
    curl_easy_cleanup(curl);
}




void HTTPReq::init(std::string url,
                   bool verbose,
                   bool errOutput)
{
    curl = curl_easy_init();

    if(curl)
    {
        // callback that writes response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResponse);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        // buffer to store errors in
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        // follow redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        setVerbose(verbose);
        setErrOutput(errOutput);
    }
}




void HTTPReq::setVerbose(const bool verbose)
{
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    this->verbose = verbose;
}


void HTTPReq::setErrOutput(const bool errOutput)
{
    this->errOutput = errOutput;
}


bool HTTPReq::getVerbose()
{
    return errOutput;
}


bool HTTPReq::getErrOutput()
{
    return errOutput;
}





time_t HTTPReq::getLastSentTimestamp()
{
    return lastSentTimestamp;
}


time_t HTTPReq::getLastResponseTimestamp()
{
    return lastResponseTimestamp;
}




void HTTPReq::send()
{
    errbuf[0] = 0;            // empty error buffer
    time(&lastSentTimestamp); // reset send intent time

    // do request!
    responseCode = curl_easy_perform(curl);

    // check for errors
    if(responseCode != CURLE_OK)
    {
        //output error
        if(errOutput)
        {
            size_t len = std::strlen(errbuf);

            std::cerr << "\nlibcurl: (" << responseCode << ") ";

            if(len)
            {
                std::cerr << errbuf;
                if(errbuf[len - 1] != '\n')
                    std::cerr << std::endl;
            }
            else
                std::cerr << curl_easy_strerror(responseCode) << std::endl;
        }

        // set response empty
        response = "";
    }
    else
    {
        time(&lastResponseTimestamp); // ok response reset time
    }
}




void HTTPReq::setUrl(const std::string url)
{
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    this->url = url;
}

/*
void HTTPReq::setUrl(const std::string url)
{
    char *urlStr = url.c_str();
    urlStr = curl_easy_escape(curl, urlStr, strlen(urlStr));
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    this->url = urlStr;
}


void HTTPReq::setUrl(char * url)
{
    url = curl_easy_escape(curl, urlStr, strlen(urlStr));
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    this->url = urlStr;
}
*/


std::string HTTPReq::getUrl()
{
    return this->url = url;
}

std::string HTTPReq::getResponse()
{
    return response;
}



size_t HTTPReq::writeResponse(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}