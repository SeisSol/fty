#ifndef PARSER_FTYIO_H
#define PARSER_FTYIO_H

#include "FtyDataTypes.hpp"
#include "FtyExceptions.hpp"
#include <yaml-cpp/yaml.h>

std::ostream &operator<<(std::ostream &Stream, const fty::StringsT &Strings);
std::ostream &operator<<(std::ostream &Stream, const fty::BlockT &Block);


namespace fty {
  namespace io {
    fty::StringsT getFileAsStrings(const std::string &FileName);
  }

  namespace internals {
    std::string getHeader(const BlockT &Block);
    YAML::Node getFields(const BlockT &Block);
    bool startsWith(const std::string &String, const char Character);
    void removeComments(StringsT &Content);
    void removeEmptyLines(StringsT &Content);
    void removeEmptyBlocks(std::list<BlockT> &Blocks);
    BlockT getNextBlock(StringsT::iterator &CurrentItr, const StringsT::iterator End);
    YAML::Node convertToYAML(fty::StringsT Content);
    YAML::Node load(const std::string& FileName);
  }
}

#endif //PARSER_FTYIO_H
