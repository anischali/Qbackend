#ifndef __web_ENGINE_H__
#define __web_ENGINE_H__
#include <string>

namespace qbackend::engines {
class web_engine {

private:

public:
    web_engine();
    ~web_engine();


    static int fetch_file(std::string host, std::string link, std::string destination);
    static int request(std::string host, std::string link, std::string *output);
};
};

#endif /* __web_ENGINE_H__ */