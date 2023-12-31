#ifndef _LANG_TRANSLATION_HPP_
#define _LANG_TRANSLATION_HPP_
#include "translation.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace qbackend::model {

    class lang_translation {

    public:
        std::string language;
        std::vector<translation *> translations;

        lang_translation();
        lang_translation(std::string language,
                   std::vector<translation *> _translations);
        ~lang_translation();

        static lang_translation *from_json(nlohmann::json const &js);
        const nlohmann::json to_json(void) const;
    };
}; // namespace qbackend

#endif /* _LANG_TRANSLATION_HPP_ */