#ifndef FTY_CONVERTER_BLOCKPARSER_HPP
#define FTY_CONVERTER_BLOCKPARSER_HPP

#include "FtyInternals.hpp"
#include <regex>
#include <sstream>

namespace fty {

template <typename Policy> class BlockParser {
public:
  std::string getHeader(const BlockT &Block) {
    std::smatch Match;
    const std::string &Header = *Block.first;
    if (std::regex_match(Header, Match, m_HeaderExpr)) {
      return m_KeyModifier.apply(Match[1]);
    } else {
      throw exception::CriticalTextBlockException("corrupted block header: " + Header);
    }
  }

  YAML::Node getFields(const BlockT &Block) {
    YAML::Node Fields;

    auto Itr = next(Block.first); // the header
    auto End = Block.second;      // the footer

    if (Itr == End) {
      throw exception::CriticalTextBlockException("provided an empty block: " + *Itr);
    }

    for (; Itr != End; ++Itr) {
      std::smatch Match;
      if (std::regex_match(*Itr, Match, m_FieldExpr)) {
        std::string Identifier = m_KeyModifier.apply(Match[1]);
        std::string ValueStr = Match[2];

        if (!Fields[Identifier]) {

          // remove quotes if any
          std::smatch SubMatch;
          Fields[Identifier] =
              (std::regex_match(ValueStr, SubMatch, m_QuotedValueExpr)) ? SubMatch[2] : ValueStr;
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

private:
  std::regex m_HeaderExpr{"^\\s*&\\s*(\\w*)\\s*.*\\s?"};
  std::regex m_FieldExpr{"\\s*(\\w*)\\s*=\\s*((?:\\w|[[:punct:]])(?:(?:\\w|[[:punct:]]|\\s)*(?:\\w|"
                         "[[:punct:]]))?)\\s*"};
  std::regex m_QuotedValueExpr{"^(\'|\")+(.*)(\'|\")+$"};
  Policy m_KeyModifier;
};
} // namespace fty

#endif // FTY_CONVERTER_BLOCKPARSER_HPP
