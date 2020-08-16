#ifndef PARSER_FTY_H
#define PARSER_FTY_H

#include "FtyDataTypes.hpp"
#include "FtyExceptions.hpp"
#include <yaml-cpp/yaml.h>

namespace fty {
  YAML::Node convertToYAML(fty::StringsT Content);
  YAML::Node load(const std::string& FileName);
}

#endif  // PARSER_FTY_H