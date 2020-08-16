#ifndef FTY_CONVERTER_BLOCKPARSER_HPP
#define FTY_CONVERTER_BLOCKPARSER_HPP


#include "FtyInternals.hpp"
#include <sstream>
#include <regex>

namespace fty {
  class BlockParser {
  public:
    std::string getHeader(const BlockT &Block) {
      std::regex Expr("&\\s*(\\w*)\\s*.*");
      std::smatch Match;
      const std::string &Header = *Block.first;
      if (std::regex_match(Header, Match, Expr)) {
        return Match[1];
      } else {
        throw exception::CriticalTextBlockException("corrupted block header: " + Header);
      }
    }

    YAML::Node getFields(const BlockT &Block) {
      YAML::Node Fields;

      std::regex Expr("\\s*(\\w*)\\s*=\\s*((?:\\w|[[:punct:]])(?:(?:\\w|[[:punct:]]|\\s)*(?:\\w|[[:punct:]]))?)\\s*");
      std::regex SubExpr("^(\'|\")+(.*)(\'|\")+$");

      auto Itr = next(Block.first); // the header
      auto End = Block.second;  // the footer


      for (; Itr != End; ++Itr) {
        std::smatch Match;
        if (std::regex_match(*Itr, Match, Expr)) {
          std::string Identifier = Match[1];
          std::string ValueStr = Match[2];

          if (!Fields[Identifier]) {

            // remove quotes if any
            std::smatch SubMatch;
            Fields[Identifier] = (std::regex_match(ValueStr, SubMatch, SubExpr)) ? SubMatch[2] : ValueStr;
          } else {
            // means that we found an identical filed in a block
            std::stringstream Stream;
            Stream << "In block (" << *Block.first << ") ";
            Stream << "found an identical field: " << *Itr;
            throw exception::CriticalKeyValueError(Stream.str());
          }
        } else {
          // means that a block string doesn't satisfy the regex
          std::stringstream Stream;
          Stream << "In block (" << *Block.first << ") ";
          Stream << "found a corrupted field: " << (*Itr);
          throw exception::CriticalTextBlockException(Stream.str());
        }
      }

      return Fields;
    }
  };
}

#endif //FTY_CONVERTER_BLOCKPARSER_HPP
