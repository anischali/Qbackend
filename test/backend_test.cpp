#include "model/lang_translation.hpp"
#include "model/translation.hpp"
#include <exception>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <json_engine.hpp>
#include <translation.hpp>
#include <lang_translation.hpp>
#include <translations.hpp>
#include <fmt/core.h>


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


void *ltsjson_decode_callback (nlohmann::json js)
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

using namespace qbackend;


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
    engine->~json_engine();
} 