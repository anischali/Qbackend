#include "web_engine.hpp"
#include <filesystem>
#include <iostream>
#include <httplib.h>
#include <fmt/core.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


using namespace qbackend::engines;

web_engine::web_engine()
{

}

web_engine::~web_engine()
{
}

int
web_engine::fetch_file(std::string host, std::string link, std::string destination)
{
    int fd = -1;
    auto req = httplib::Client(fmt::format("{}:{}", host, host.find("https") != std::string::npos ? "443" : "80")).Get(link);
    if (req != nullptr && req->status == 200)
    {
        fd = open(destination.c_str(), O_WRONLY | O_CREAT, 0666);
        if (fd == -1)
        {
            return -ENODEV;
        }

        write(fd, req->body.c_str(), req->body.size());
        
        close(fd);
    }
    else
    {
        return -EINVAL;
    }

    return 0;
}