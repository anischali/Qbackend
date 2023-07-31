#include "lang_translation.hpp"
#include "translation.hpp"
#include <functional>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fmt/core.h>


using namespace qbackend::model;


lang_translation::lang_translation() :
    language(std::string("")),
    translations(std::vector<translation *>())
{}


lang_translation::lang_translation(std::string language, 
                        std::vector<translation *>translations) :
    language(language),
    translations(translations)
{}



lang_translation::~lang_translation()
{
    for (auto &translation : translations)
    {
        delete translation;
    }
}


lang_translation * 
lang_translation::from_json(nlohmann::json const &js)
{
    lang_translation *lt = new lang_translation();
    lt->language = js["language"];
    auto ts = js["translations"];
    for (auto &t : ts)
    {
        lt->translations.push_back(translation::from_json(t));
    } 
    
    return lt;  
}


const nlohmann::json lang_translation::to_json(void) const
{
    throw std::logic_error("lang_translation::to_json(): not implemented");
}
