// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#ifndef FTY_CONVERTER_BLOCKPARSER_HPP
#define FTY_CONVERTER_BLOCKPARSER_HPP

#include "FtyInternals.hpp"
#include <regex>
#include <sstream>

namespace fty {

template <typename Policy>
class BlockParser {
  public:
  auto getHeader(const BlockT& Block) -> std::string {
    std::smatch Match;
    const std::string& Header = *Block.first;
    if (std::regex_match(Header, Match, m_HeaderExpr)) {
      return m_KeyModifier.apply(Match[1]);
    } else {
      throw exception::CriticalTextBlockException("corrupted block header: " + Header);
    }
  }

  auto getFields(const BlockT& Block) -> YAML::Node {
    YAML::Node Fields;

    auto Itr = next(Block.first); // the header
    auto End = Block.second;      // the footer

    if (Itr == End) {
      throw exception::CriticalTextBlockException("provided an empty block: " + *Itr);
    }

    for (; Itr != End; ++Itr) {
      std::smatch Match;
      if (std::regex_match(*Itr, Match, m_FieldExpr)) {
        const std::string Identifier = m_KeyModifier.apply(Match[1]);
        const std::string ValueStr = Match[2];

        if (!Fields[Identifier]) {

          // remove quotes if any or adjust legacy fortran floating point format
          std::smatch SubMatch;
          if (std::regex_match(ValueStr, SubMatch, m_QuotedValueExpr)) {
            Fields[Identifier] = std::string(SubMatch[2]);
          } else if (std::regex_match(ValueStr, SubMatch, m_LegacyFortranFloatExpr)) {
            Fields[Identifier] = std::regex_replace(ValueStr, m_LegacyFortranFloatExpr, "$1e$2");
          } else {
            Fields[Identifier] = ValueStr;
          }
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
  std::regex m_HeaderExpr{R"(^\s*&\s*(\w*)\s*.*\s?)"};
  std::regex m_FieldExpr{
      R"(\s*(\w*)\s*=\s*((?:\w|[[:punct:]])(?:(?:\w|[[:punct:]]|\s)*(?:\w|[[:punct:]]))?)\s*)"};
  std::regex m_QuotedValueExpr{R"(^('|")+(.*)('|")+$)"};
  std::regex m_LegacyFortranFloatExpr{
      R"(^(\d*\.?\d*)(?:D|d)([\+-]?\d+)$)"}; // 'D' or 'd' instead of 'E' or 'e'
  Policy m_KeyModifier;
};
} // namespace fty

#endif // FTY_CONVERTER_BLOCKPARSER_HPP
