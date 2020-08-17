#ifndef FTY_CONVERTER_POLICIES_HPP
#define FTY_CONVERTER_POLICIES_HPP

#include <string>
#include <algorithm>

namespace fty {
  struct As_lowercase {
    std::string apply(const std::string& String) {
      std::string ConvertedString(String.size(), '\0');
      std::transform(String.begin(), String.end(), ConvertedString.begin(),
                     [](unsigned char Char) {return std::tolower(Char);});
      return ConvertedString;
    }
  };

  struct As_uppercase {
    std::string apply(const std::string& String) {
      std::string ConvertedString(String.size(), '\0');
      std::transform(String.begin(), String.end(), ConvertedString.begin(),
                     [](unsigned char Char) {return std::toupper(Char);});
      return ConvertedString;
    }
  };

  struct As_default {
    std::string apply(const std::string& String) {
      return String;
    }
  };
}

#endif //FTY_CONVERTER_POLICIES_HPP