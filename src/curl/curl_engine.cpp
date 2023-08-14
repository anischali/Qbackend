#include "curl_engine.hpp"
#include <filesystem>
#include <iostream>


using namespace qbackend::engines;

curl_engine::curl_engine()
{
    curl = curl_easy_init();

}

curl_engine::~curl_engine()
{
    curl_free(curl);
}

void
curl_engine::fetch_file(std::string url, std::string filename)
{

}