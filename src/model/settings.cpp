#include "settings.hpp"
#include <exception>
#include <functional>
#include <map>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fmt/core.h>




using namespace qbackend::model;
using namespace qbackend::engines;

settings::settings()
{

    e = new qbackend::engines::json_engine();
}

settings::~settings()
{
    delete e;
    qbackend::model::settings::e = nullptr;
}


settings * 
settings::from_json(nlohmann::json const &js)
{
    
    if (!js.contains("id") ||
        !js.contains("language") ||
        !js.contains("path"))
            throw std::invalid_argument(std::string("json data corrupted"));

    settings *stg = new settings();
    
    stg->id = js["id"];
    stg->language = js["language"];
    stg->path = js["path"];

    return stg;  
}


const nlohmann::json settings::to_json(void) const
{
    std::map<std::string, std::string> m = {
        {"id", this->id},
        {"language", this->language},
        {"path", this->path}
    };

    return nlohmann::json(m);
}



void * settings::settings_load_callback(nlohmann::json &js)
{
    return settings::from_json(js);
}

nlohmann::json settings::settings_save_callback(const void *obj)
{
    std::cout << "Setting" << std::endl;
    nlohmann::json js = ((settings *)obj)->to_json();
    
    std::cout << js.dump(4) << std::endl;

    return js;
}


void 
settings::load(std::string filename)
{
    settings *s = (settings *)e->json_load(filename, settings::settings_load_callback);

    this->id = s->id;
    this->language = s->language;
    this->path = s->path;

    delete s;
}


void 
settings::save(std::string filename)
{
    std::cout << filename << std::endl;
    e->json_save(this, filename, settings::settings_save_callback);
}