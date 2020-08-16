#ifndef FTY_CONVERTER_FTY_HPP
#define FTY_CONVERTER_FTY_HPP


#include "FtyInternals.hpp"
#include "StringProcessor.h"
#include "BlockProcessor.hpp"
#include "BlockParser.hpp"
#include <iostream>
#include <fstream>

namespace fty {

  class Converter {
  public:
    YAML::Node convertToYAML(fty::StringsT Content) {

      m_StringProcessor.removeComments(Content);
      m_StringProcessor.removeEmptyLines(Content);


      StringsT::iterator Begin = Content.begin();
      StringsT::iterator End = Content.end();

      std::list<BlockT> Blocks;
      try {

        while (Begin != End) {
          Blocks.push_back(m_BlockProcessor.getNextBlock(Begin, End));
        }
      }
      catch (const exception::TextBlockException& Error) {
        m_Warnings.push_back(Error.what());
        // a file can be parsed further. Recoverable error
      }
      catch (const exception::CriticalTextBlockException& Error) {
        std::cout << Error.what() << std::endl;
        throw Error;
      }
      catch (const std::exception& Error) {
        std::cout << Error.what() << std::endl;
        throw Error;
      }

      m_BlockProcessor.removeEmptyBlocks(Blocks);

      YAML::Node Params;
      for (auto& Block: Blocks) {
        std::string&& Header = m_BlockParser.getHeader(Block);

        if (!Params[Header]) {
          Params[Header] = m_BlockParser.getFields(Block);
        }
        else {
          throw exception::CriticalKeyValueError("Found an identical field (header): " + Header);
        }
      }

      return Params;
    }

    StringsT getWarnings() {
      return m_Warnings;
    }

  private:

    StringProcessor m_StringProcessor;
    BlockProcessor m_BlockProcessor;
    BlockParser m_BlockParser;
    StringsT m_Warnings;
  };


  class Loader {
  public:
    YAML::Node load(const std::string& FileName) {
      StringsT Content{};
      try {
        Content = getFileAsStrings(FileName);
      }
      catch (const exception::FileException& Error) {
        std::cerr << Error.what() << std::endl;
        throw Error;
      }
      catch (const std::exception& Error) {
        std::cerr << Error.what() << std::endl;
        throw Error;
      }

      YAML::Node Params = m_Converter.convertToYAML(Content);

      return Params;
    }


    StringsT getWarningsFromConverter() {
      return m_Converter.getWarnings();
    }

  private:
    StringsT getFileAsStrings(const std::string &FileName) {
      fty::StringsT Content;
      std::fstream ParamFile(FileName, std::ios_base::in);
      if (!ParamFile.is_open()) {
        throw exception::FileException("cannot open file: " + FileName);
      }

      std::string TmpString;
      while (std::getline(ParamFile, TmpString)) {
        Content.push_back(TmpString);
      }

      ParamFile.close();
      return Content;
    }


    Converter m_Converter;
  };
}

#endif //FTY_CONVERTER_FTY_HPP
