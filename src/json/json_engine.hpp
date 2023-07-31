#ifndef _JSON_ENGINE_HPP
#define _JSON_ENGINE_HPP
#include <nlohmann/json.hpp>




namespace qbackend::engines {
    class json_engine {
    public:
        json_engine();

        ~json_engine();


        nlohmann::json encode(const void *obj, nlohmann::json (*callback)(const void *obj));
        void *decode(const char *json, void *(*callback)(nlohmann::json &js));
    
        void *json_load(const std::string filename, void *(*callback)(nlohmann::json &js));
        int json_save(const void *obj, const std::string filename, nlohmann::json (*callback)(const void *obj));

    };
};

#endif // JSON_ENGINE_HPP