#ifndef __TRANSLATION_HPP_
#define __TRANSLATION_HPP_
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace qbackend::model {

class translation {

public:
  translation();
  ~translation();
  translation(std::string const &_name, std::string const &_translator_name,
              std::string const &_translation_desc,
              std::string const &_translator_page, std::string const &_url);

  static translation *from_json(nlohmann::json const &js);
  const nlohmann::json to_json(void) const;

  std::string name, translator_name, translation_desc, translator_page, url;
};

}; // namespace qbackend::model
#endif