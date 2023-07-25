#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <json_engine.hpp>


void *json_decode_callback (nlohmann::json js)
{
    std::cout << js.dump(4) << std::endl;
    return nullptr;
}

using namespace qbackend;


int main(int argc, char **argv)
{
    json_engine *engine = new json_engine();
    engine->json_load(argv[1], json_decode_callback);
    printf("Usage: test\n");
    engine->~json_engine();
} 