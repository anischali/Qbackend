#include "settings.hpp"
#include <cstddef>
#include <exception>
#include <functional>
#include <map>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fmt/core.h>
#include <map>
#include <storage_engine.hpp>




using namespace qbackend::model;
using namespace qbackend::engines;

settings::settings(std::string path, std::string name)
{
    e = new json_engine();
    this->path = path;
    this->name = name;

    if (!storage_engine::path_exist(this->path))
        storage_engine::create_directory(this->path, true);

    if (storage_engine::file_exist(get_filename()))
    {
        load();
    }
}

settings::~settings()
{
    delete e;
    this->e = nullptr;
}


std::map<std::string, std::string> *
settings::from_json(nlohmann::json const &js)
{   
    if (js.empty())
        return nullptr;

    std::map<std::string, std::string> map = (std::map<std::string, std::string>)js.get<std::map<std::string, std::string>>();
    return new std::map<std::string, std::string>(map.begin(), map.end());
}


const nlohmann::json settings::to_json(void) const
{
    return nlohmann::json(m_settings);
}



void * settings::settings_load_callback(nlohmann::json &js)
{
    return (void *)settings::from_json(js);
}

nlohmann::json settings::settings_save_callback(const void *obj)
{
    nlohmann::json js = ((settings *)obj)->to_json();
    return js;
}


void 
settings::load()
{
    auto s = (std::map<std::string, std::string> *)e->json_load(get_filename(), settings::settings_load_callback);
    if (s != nullptr)
    {
        this->m_settings = std::map<std::string, std::string>(s->begin(), s->end());
        delete s;
    }
}


void 
settings::save()
{
    e->json_save(this, get_filename(), settings::settings_save_callback);
}



std::string settings::get_filename() const
{
    return fmt::format("{}/{}.json", this->path, this->name);
}



void settings::set_value(const std::string key, const std::string value)
{
    this->m_settings[key] = value;
    this->save();
}


std::string settings::get_value(std::string key)
{
    if (this->m_settings.find(key) != this->m_settings.end())  
    {
        return this->m_settings[key];
    }

    return "";
}