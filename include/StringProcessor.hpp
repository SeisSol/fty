// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#ifndef FTY_CONVERTER_STRINGPROCESSOR_HPP
#define FTY_CONVERTER_STRINGPROCESSOR_HPP

#include "FtyInternals.hpp"
#include <regex>

namespace fty {
class StringProcessor {
  public:
  /**
   * Checks whether the given line starts with 'Character'
   * @param String tests string
   * @param Character tests character
   * @return true if the string starts with 'Character'. Otherwise, false
   */
  bool startsWith(const std::string& String, const char Character) {
    auto CurrentChar = String.begin();

    // find the first non-white-space character
    while ((CurrentChar != String.end()) && (isspace(*(CurrentChar)))) {
      ++CurrentChar;
    };

    return ((*CurrentChar) == Character) && (CurrentChar != String.end());
  }

  void removeComments(StringsT& Content) {

    for (auto& Item : Content) {
      std::smatch Match;
      if (std::regex_match(Item, Match, m_CommentExpr)) {
        Item = Match[1];
      }
    }
  }

  void removeEmptyLines(StringsT& Content) {

    const std::string WHITESPACE = " \n\r\t\f\v";
    auto IsEmptyString = [&WHITESPACE](const std::string& String) -> bool {
      const size_t Start = String.find_first_not_of(WHITESPACE);
      return Start == std::string::npos;
    };

    std::vector<StringsT::iterator> Deletees;
    for (auto Itr = Content.begin(); Itr != Content.end(); ++Itr) {
      if (IsEmptyString(*Itr))
        Deletees.push_back(Itr);
    }

    for (auto& Itr : Deletees) {
      Content.erase(Itr);
    }
  }

  private:
  std::regex m_CommentExpr{"^([^!]*)!.*\\s?$"};
};
} // namespace fty

#endif // FTY_CONVERTER_STRINGPROCESSOR_HPP
