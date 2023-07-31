#ifndef __SETTINGS_HPP_
#define __SETTINGS_HPP_
#include <map>
#include <nlohmann/json.hpp>
#include <json_engine.hpp>
#include <string>
#include <vector>

namespace qbackend::model {

    class settings {
    
    private:
        qbackend::engines::json_engine *e = nullptr;
        static void * settings_load_callback(nlohmann::json &js);
        static nlohmann::json settings_save_callback(const void *obj);

    public:
        settings();
        ~settings();

        static settings *from_json(nlohmann::json const &js);
        const nlohmann::json to_json(void) const;



        void load(std::string filename);
        void save(std::string filename);

        std::string id, language, path;
    };

}; // namespace qbackend::model
#endif