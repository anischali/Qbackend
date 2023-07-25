#ifndef _JSON_ENGINE_HPP
#define _JSON_ENGINE_HPP
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <nlohmann/json.hpp>




namespace qbackend {
    class json_engine {
    public:
        json_engine();

        ~json_engine();


        char *encode(const void *obj, char *(*callback)(const void *obj));
        void *decode(const char *json, void *(*callback)(nlohmann::json js));
    
        void *json_load(const char *filename, void *(*callback)(nlohmann::json js));
        int json_save(const void *obj, const char *filename, char *(*callback)(const void *obj));

    };
};

#endif // JSON_ENGINE_HPP