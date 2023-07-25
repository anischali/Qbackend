#include "model/translation.hpp"
#include <exception>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <json_engine.hpp>
#include <translation.hpp>
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

using namespace qbackend;


int main(int argc, char **argv)
{
    translation *o;
    json_engine *engine = new json_engine();
    o = (translation *)engine->json_load(argv[1], json_decode_callback);
    printf("url: %s\n", fmt::format(o->url, o->name).c_str());
    delete o;
    engine->~json_engine();
} 