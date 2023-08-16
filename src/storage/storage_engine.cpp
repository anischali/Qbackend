#include "storage_engine.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fmt/core.h>


using namespace qbackend::engines;


storage_engine::storage_engine() {}

storage_engine::~storage_engine() {}

bool storage_engine::file_exist(std::string filename) {
    struct stat st;

    if (stat(filename.c_str(), &st) < 0)
        return false;

    return (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode));
}



bool storage_engine::path_exist(std::string pathname)
{
    struct stat st;

    if (stat(pathname.c_str(), &st) < 0)
        return false;

    return S_ISDIR(st.st_mode);
}



int storage_engine::create_directory(std::string pathname, bool recursive)
{
    int ret = 0;
    char *pname = (char *)pathname.c_str(), *ptr, *token, *save_ptr;
    ptr = token = save_ptr = nullptr;
    struct stat st;
    std::string subdirs = (pname[0] == '/') ? "/" : "";

    if (!recursive && stat(token, &st) < 0)
    {
        return mkdir(pname, 0777);
    }

    for (ptr = pname; !ret; ptr = NULL)
    {
        token = strtok_r(ptr, "/", &save_ptr);
        if (token == nullptr)
            break;
        
        subdirs += fmt::format("{}/", token);
        if (stat(subdirs.c_str(), &st) < 0) {
            ret = mkdir(subdirs.c_str(), 0777);
        }
    }

    return ret;
}