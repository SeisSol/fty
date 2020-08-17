#ifndef FTY_CONVERTER_STRINGPROCESSOR_H
#define FTY_CONVERTER_STRINGPROCESSOR_H

#include "FtyInternals.hpp"
#include "FtyInternals.hpp"
#include <regex>

namespace fty {
  class StringProcessor {
  public:

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

      for (auto &Item : Content) {
        std::smatch Match;
        if (std::regex_match(Item, Match, m_Comment_Expr)) {
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

  private:
    std::regex m_Comment_Expr{"^([^!]*)!.*"};
  };
}

#endif //FTY_CONVERTER_STRINGPROCESSOR_H
