#include "translation.hpp"
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fmt/core.h>


const static std::string json_fmt = std::string("{\"name\": \"%s\",\"translator_name\": \"%s\",\"translation_desc\": \"%s\",\"translator_page\": \"%s\",\"url\": \"%s\"}");



translation::translation() :
    name(std::string("")), translator_name(std::string("")),
    translation_desc(std::string("")),
    translator_page(std::string("")),
    url(std::string(""))
{}

translation::translation(std::string const& _name, 
                std::string const& _translator_name, 
                std::string const& _translation_desc, 
                std::string const& _translator_page, 
                std::string const& _url) : 
                name(_name), translator_name(_translator_name),
                translation_desc(_translation_desc),
                translator_page(_translator_page),
                url(_url)
{}





translation::~translation()
{
}


translation * 
translation::from_json(nlohmann::json const &js)
{
    
    if (!js.contains("name") ||
        !js.contains("translator_name") ||
        !js.contains("translation_desc") ||
        !js.contains("translator_page") ||
        !js.contains("url"))
            throw std::invalid_argument(std::string("json data corrupted"));

    return new translation(
            js["name"], 
            js["translator_name"], 
            js["translation_desc"], 
            js["translator_page"], 
            js["url"]);  
}


const nlohmann::json translation::to_json(void) const
{
    return nlohmann::json::parse(
        fmt::format(json_fmt, 
            name, translator_name, 
            translation_desc, 
            translator_page, 
            url)
    );
}




