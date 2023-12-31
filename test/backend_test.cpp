#include <lang_translation.hpp>
#include <translation.hpp>
#include <exception>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <json_engine.hpp>
#include <storage_engine.hpp>
#include <translation.hpp>
#include <lang_translation.hpp>
#include <translations.hpp>
#include <string>
#include <fmt/core.h>
#include <settings.hpp>

using namespace qbackend::engines;
using namespace qbackend::model;




void *json_decode_callback (nlohmann::json js)
{
    translation *t;
    try {
        std::cout << js.dump(4) << std::endl;
        t = translation::from_json(js);
    }catch(std::exception &e)
    {
        return nullptr;
    }
        
    return (void *)t;
}


void *ltjson_decode_callback (nlohmann::json js)
{
    lang_translation *t;
    try {
        std::cout << js.dump(4) << std::endl;
        t = lang_translation::from_json(js);
    }catch(std::exception &e)
    {
        return nullptr;
    }
        
    return (void *)t;
}


void *ltsjson_decode_callback (nlohmann::json &js)
{
    translations *t;
    try {
        std::cout << js.dump(4) << std::endl;
        t = translations::from_json(js);
    }catch(std::exception &e)
    {
        return nullptr;
    }
        
    return (void *)t;
}



int main(int argc, char **argv)
{
    settings *s2 = new settings("/home/anicha1/.config/org.test.com", "quran_app_settings_2");
    s2->set_value("language", "en");
    s2->set_value("scale_factor", std::to_string(3));
    
    std::cout << s2->path << " " << s2->name << std::endl;

    web_engine *e = new web_engine();

    web_engine::fetch_file("https://tanzil.net", fmt::format("/trans/?transID={}&type=xml", "sq.nahi"), "/tmp/sq.nahi.xml");


    std::cout << storage_engine::create_directory(argv[1], true) << std::endl;

    delete s2;
    delete e;
} 