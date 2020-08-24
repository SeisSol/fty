#ifndef PARSER_FTYDATATYPES_HPP
#define PARSER_FTYDATATYPES_HPP

#include <list>
#include <string>

namespace fty {
using StringsT = std::list<std::string>;
using BlockT = std::pair<StringsT::iterator, StringsT::iterator>;
} // namespace fty

#endif // PARSER_FTYDATATYPES_HPP
