#ifndef __SETTINGS_HPP_
#define __SETTINGS_HPP_
#include <map>
#include <nlohmann/json.hpp>
#include "json_engine.hpp"
#include <string>
#include <vector>

namespace qbackend::model {

    class settings {
    
    private:
        std::map<std::string, std::string> m_settings = std::map<std::string, std::string>();
        qbackend::engines::json_engine *e = nullptr;
        static void * settings_load_callback(nlohmann::json &js);
        static nlohmann::json settings_save_callback(const void *obj);

        void load();
        void save();

    public:
        std::string name, path;
        settings(std::string path, std::string name);
        ~settings();

        static std::map<std::string, std::string> * from_json(nlohmann::json const &js);
        const nlohmann::json to_json(void) const;

        std::string get_filename() const;


        std::string get_value(std::string key);
        void set_value(std::string key, std::string value);
    };
}; // namespace qbackend::model
#endif