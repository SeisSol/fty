// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#ifndef FTY_CONVERTER_POLICIES_HPP
#define FTY_CONVERTER_POLICIES_HPP

#include <algorithm>
#include <string>

namespace fty {
struct AsLowercase {
  static auto apply(const std::string& String) -> std::string {
    std::string ConvertedString(String.size(), '\0');
    std::transform(String.begin(), String.end(), ConvertedString.begin(), [](unsigned char Char) {
      return std::tolower(Char);
    });
    return ConvertedString;
  }
};

struct AsUppercase {
  static auto apply(const std::string& String) -> std::string {
    std::string ConvertedString(String.size(), '\0');
    std::transform(String.begin(), String.end(), ConvertedString.begin(), [](unsigned char Char) {
      return std::toupper(Char);
    });
    return ConvertedString;
  }
};

struct AsOriginal {
  static auto apply(const std::string& String) -> std::string { return String; }
};
} // namespace fty

#endif // FTY_CONVERTER_POLICIES_HPP
