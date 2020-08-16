#ifndef PARSER_FTYSTRINGPROCESSING_H
#define PARSER_FTYSTRINGPROCESSING_H

#include "FtyInternals.h"
#include <regex>

namespace fty {
  namespace internals {

    /**
     * Checks whether the given line starts with 'Character'
     * @param String test string
     * @param Character test character
     * @return true if the string starts with 'Character'. Otherwise, false
     */
    bool startsWith(const std::string &String, const char Character) {
      auto CurrentChar = String.begin();

      // find the first non-white-space character
      while ((CurrentChar != String.end()) && (isspace(*(CurrentChar)))) {
        ++CurrentChar;
      };

      return ((*CurrentChar) == Character) && (CurrentChar != String.end());

    }


    void removeComments(StringsT &Content) {
      std::regex Expr("^([^!]*)!.*");
      for (auto &Item : Content) {
        std::smatch Match;
        if (std::regex_match(Item, Match, Expr)) {
          Item = Match[1];
        }
      }
    }


    void removeEmptyLines(StringsT &Content) {

      const std::string WHITESPACE = " \n\r\t\f\v";
      auto isEmptyString = [&WHITESPACE](const std::string &String) -> bool {
        size_t Start = String.find_first_not_of(WHITESPACE);
        return Start == std::string::npos;
      };

      std::vector<StringsT::iterator> Deletees;
      for (auto Itr = Content.begin(); Itr != Content.end(); ++Itr) {
        if (isEmptyString(*Itr))
          Deletees.push_back(Itr);
      }

      for (auto &Itr: Deletees) {
        Content.erase(Itr);
      }
    }
  }
}

#endif //PARSER_FTYSTRINGPROCESSING_H
