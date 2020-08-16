#ifndef PARSER_FTYEXCEPTIONS_HPP
#define PARSER_FTYEXCEPTIONS_HPP

#include <string>
#include <stdexcept>

namespace fty {
  namespace exception {

    class BaseException : public std::runtime_error {
    public:
      BaseException(const std::string& Message) : std::runtime_error("ParsingError: " + Message) {}
    };

    class FileException : public BaseException {
    public:
      FileException(const std::string& Message) : BaseException(Message) {}
    };

    class TextBlockException : public BaseException {
    public:
      TextBlockException(const std::string& Message) : BaseException(Message) {}
    };

    class CriticalTextBlockException : public BaseException {
    public:
      CriticalTextBlockException(const std::string& Message) : BaseException(Message) {}
    };


    class CriticalKeyValueError : public BaseException {
    public:
      CriticalKeyValueError(const std::string& Message) : BaseException(Message) {}
    };

  }
}

#endif //PARSER_FTYEXCEPTIONS_HPP
