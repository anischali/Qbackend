#include <lang_translation.hpp>
#include <translation.hpp>
#include <exception>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <json_engine.hpp>
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
    //translation *o;
    translations *lts;

    json_engine *engine = new json_engine();
    //o = (translation *)engine->json_load(argv[1], json_decode_callback);
    lts = (translations *)engine->json_load(argv[1], ltsjson_decode_callback);

    printf("tss: %ld\n", lts->supported_translations.size());
    //delete o;
    delete lts;
    delete engine;

    settings *s = new settings();

    s->id = "quran_app_settings";
    s->language = "en";
    s->path = "/home/anicha1/tmp";
    std::cout << s->path << std::endl;
    s->save(fmt::format(std::string("{}/{}.json"), s->path, s->id));
    delete s;

    settings *s2 = new settings();
    s2->load(fmt::format(std::string("{}/{}.json"), "/home/anicha1/tmp", "quran_app_settings"));

    std::cout << s2->path << " " << s2->id << std::endl;

    delete s2;
} 