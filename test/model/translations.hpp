#ifndef __TRANSLATIONS_HPP_
#define __TRANSLATIONS_HPP_
#include <lang_translation.hpp>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>




class translations {
    
public:
    std::vector<lang_translation *> supported_translations;
    
    translations();
    ~translations();

    static translations * from_json(nlohmann::json const &js);
    const nlohmann::json to_json(void) const;

};


#endif