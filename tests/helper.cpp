#include "helper.hpp"

BlockT make_block(StringsT& Content) {
  return std::make_pair(Content.begin(), --Content.end());
}