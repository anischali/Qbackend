#include "json_engine.hpp"
#include <asm-generic/errno-base.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <json-c/json.h>


using namespace qbackend;



json_engine::json_engine()
{

}

json_engine::~json_engine()
{

}


void* 
json_engine::json_load(const char *filename, void *(*callback) (const char *json))
{
    void *obj = nullptr;
    char *str = nullptr;
    size_t size = 0;

    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        return nullptr;

    if (!callback)
        goto out;

    size = lseek(fd, 0, SEEK_END);
    str = (char *)calloc(size, sizeof(*str));
    lseek(fd, 0, SEEK_SET);
    
    if ((read(fd, str, (size_t)(size * sizeof(*str)))) <= 0)
        goto free_mem;

    obj = this->decode(str, callback);

free_mem:
    free(str);
    str = nullptr;
out:
    close(fd);
    return obj;
}


char *
json_engine::encode(const void *obj, char *(*callback)(const void *obj))
{

};

void *
json_engine::decode(const char *json, void *(*callback)(const char *json))
{
    if (!callback || !json)
        return nullptr;

    return nullptr;
}

int
json_engine::json_save(const void *obj, const char *filename, char *(*callback)(const void *obj))
{
    int fd = -1;
    int err = 0;
    char *str = nullptr;

    fd = open(filename, O_WRONLY);
    if (fd < 0)
        return -EINVAL;

    if (!callback || !obj)
        goto out;

    str = this->encode(obj, callback);
    if (!str)
        goto out;

    if ((write(fd, str, strlen(str)) <= 0))
    {
        err = -EIO;
        goto out;
    }

out:
    close(fd);
    return err;
}
