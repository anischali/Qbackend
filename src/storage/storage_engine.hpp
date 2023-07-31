#ifndef __STORAGE_ENGINE_H__
#define __STORAGE_ENGINE_H__
#include <string>

namespace qbackend::engines {
    class storage_engine {

    public:
        storage_engine();
        ~storage_engine();

        static bool file_exist(std::string filename);
        static bool path_exist(std::string path);
    
        static void create_directory(std::string path, bool recursive);
        static void create_file(std::string filename, int mode);
    };
};

#endif /* __STORAGE_ENGINE_H__ */