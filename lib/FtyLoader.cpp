/*

#include <string>
#include <list>
#include <cstdlib>
#include <yaml-cpp/yaml.h>

#include "FtyBlockParsing.cpp"
#include "FtyBlockProcessing.cpp"
#include "FtyStringProcessing.cpp"
#include "FtyInternals.h"
*/
#include <iostream>

#include "FtyInternals.h"

namespace fty {
  YAML::Node convertToYAML(fty::StringsT Content) {

    fty::internals::removeComments(Content);
    fty::internals::removeEmptyLines(Content);


    fty::StringsT::iterator Begin = Content.begin();
    fty::StringsT::iterator End = Content.end();

    std::list<fty::BlockT> Blocks;
    try {

      while (Begin != End) {
        Blocks.push_back(fty::internals::getNextBlock(Begin, End));
      }
    }
    catch (const fty::exception::TextBlockException& Error) {
      // a file can be parsed further. Recoverable error
    }
    catch (const fty::exception::CriticalTextBlockException& Error) {
      std::cerr << "ParsingError: input file cannot be processed\n";
      std::cout << Error.what() << std::endl;
      throw Error;
    }
    catch (const std::exception& Error) {
      std::cout << Error.what() << std::endl;
      throw Error;
    }

    fty::internals::removeEmptyBlocks(Blocks);

    YAML::Node Params;
    for (auto& Block: Blocks) {
      std::string&& Header = fty::internals::getHeader(Block);

      if (!Params[Header]) {
        Params[Header] = fty::internals::getFields(Block);
      }
      else {
        throw fty::exception::CriticalKeyValueError("Found an identical field (header): " + Header);
      }
    }

    return Params;
  }


  YAML::Node load(const std::string& FileName) {
    fty::StringsT Content{};
    try {
      Content = fty::io::getFileAsStrings(FileName);
    }
    catch (const fty::exception::FileException& Error) {
      std::cerr << Error.what() << std::endl;
      throw Error;
    }
    catch (const std::exception& Error) {
      std::cerr << Error.what() << std::endl;
      throw Error;
    }

    YAML::Node Params = convertToYAML(Content);

    return Params;
  }
}