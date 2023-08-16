#ifndef __STORAGE_ENGINE_H__
#define __STORAGE_ENGINE_H__
#include <string>

namespace qbackend::engines {
    class storage_engine {

    public:
        storage_engine();
        ~storage_engine();

        static bool file_exist(std::string filename);
        static bool path_exist(std::string pathname);
    
        static int create_directory(std::string pathname, bool recursive);
        static int create_file(std::string filename, int mode);
    };
};

#endif /* __STORAGE_ENGINE_H__ */