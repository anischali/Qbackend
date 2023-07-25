#ifndef __TRANSLATION_HPP_
#define __TRANSLATION_HPP_
#include <map>
#include <vector>
#include <string>




class translation {
private:
    std::map<std::string,std::string> _translation;
public:
    translation();
    ~translation();
    translation(std::string name, 
                std::string translator_name, 
                std::string translation_desc, 
                std::string translation_page, 
                std::string url);


    static translation *from_json(const char *json);
    const char *to_json(void);

const char *name, *translator_name, *translation_desc, *translation_page, *url;
};


class lang_tranlation {

private:
    std::string language;
    std::vector<translation> _translations;

public:
    lang_tranlation();
    lang_tranlation(std::string language, std::vector<translation> translations);
    ~lang_tranlation();    

    static lang_tranlation *from_json(const char *json);
    const char *to_json(void);
};

#endif