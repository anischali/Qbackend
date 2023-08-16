#include "storage_engine.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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



