#ifndef __TRANSLATION_HPP_
#define __TRANSLATION_HPP_
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>




class translation {
    
public:
    translation();
    ~translation();
    translation(std::string const& _name, 
                std::string const& _translator_name, 
                std::string const& _translation_desc, 
                std::string const& _translator_page, 
                std::string const& _url);


    static translation * from_json(nlohmann::json const &js);
    const nlohmann::json to_json(void) const;

    std::string name, translator_name, translation_desc, translator_page, url;
};


class lang_tranlation {

private:
    std::string language;
    std::vector<translation *> _translations;

public:
    lang_tranlation();
    lang_tranlation(std::string language, std::vector<translation> translations);
    ~lang_tranlation();    

    static lang_tranlation from_json(nlohmann::json js);
    const nlohmann::json to_json(void) const;
};

#endif