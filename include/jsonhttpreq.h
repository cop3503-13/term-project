#ifndef JSONHTTPREQ_H
#define JSONHTTPREQ_H

#include <string>
#include "httpreq.h"
#include "json.hpp"

class JSONHTTPReq : public HTTPReq
{


public :

    JSONHTTPReq();
    ~JSONHTTPReq();

    nlohmann::json getJSONResponse();
};

#endif //JSONHTTPREQ_H
