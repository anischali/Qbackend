#ifndef __CURL_ENGINE_H__
#define __CURL_ENGINE_H__
#include <string>

class curl_engine {

public:
    curl_engine();
    ~curl_engine();


    static void fetch_file(std::string url, std::string filename);
};


#endif /* __CURL_ENGINE_H__ */