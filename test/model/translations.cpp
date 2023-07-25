#include "lang_translation.hpp"
#include "translations.hpp"

const static std::string json_fmt = std::string("{\"name\": \"%s\",\"translator_name\": \"%s\",\"translation_desc\": \"%s\",\"translator_page\": \"%s\",\"url\": \"%s\"}");



translations::translations() :
    supported_translations(std::vector<lang_translation *>())
{}





translations::~translations()
{
    for (auto &translation : translations::supported_translations)
    {
        delete translation;
    }
}


translations * 
translations::from_json(nlohmann::json const &js)
{
    translations *tss = new translations();
    
    if (!js.contains("translations"))
            throw std::invalid_argument(std::string("no translations found"));

    auto tsa = js["translations"];

    for (auto &ts : tsa)
    {
        tss->supported_translations.push_back(lang_translation::from_json(ts));
    }

    return tss;
}


const nlohmann::json translations::to_json(void) const
{
    throw std::logic_error("translations::to_json(): not implemented");
}




