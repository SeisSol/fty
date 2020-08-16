#include "Fty.hpp"
#include <iostream>
#include <cstdlib>

int main(int Argc, char *Argv[]) {
  if (Argc != 2) {
    std::cerr << "Error: Please, provide an input file\n";
    std::cerr << "Example: fty-converter [input file path]\n";
    exit(EXIT_FAILURE);
  }

  std::string FileName = Argv[1];
  fty::Loader Loader{};
  try {
    YAML::Node Params = Loader.load(FileName);
    std::cout << Params;
  }
  catch (const fty::exception::FileException& Error) {
    std::cerr << Error.what() << std::endl;
  }
  catch (const std::exception& Error) {
    std::cerr << Error.what() << std::endl;
    throw Error;
  }
  return 0;
}