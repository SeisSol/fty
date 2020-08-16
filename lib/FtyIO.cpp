#include "FtyInternals.h"
#include <fstream>

std::ostream &operator<<(std::ostream &Stream, const fty::StringsT &Strings) {
  for (const auto &String : Strings) {
    Stream << String << std::endl;
  }

  return Stream;
}


std::ostream &operator<<(std::ostream &Stream, const fty::BlockT &Block) {
  for (auto Itr = Block.first; Itr != Block.second; ++Itr) {
    Stream << *Itr << std::endl;
  }
  return Stream;
}


namespace fty {
  namespace io {

    fty::StringsT getFileAsStrings(const std::string &FileName) {
      fty::StringsT Content;
      std::fstream ParamFile(FileName, std::ios_base::in);
      if (!ParamFile.is_open()) {
        throw fty::exception::FileException("cannot open file: " + FileName);
      }

      std::string TmpString;
      while (std::getline(ParamFile, TmpString)) {
        Content.push_back(TmpString);
      }

      ParamFile.close();
      return Content;
    }

  }
}