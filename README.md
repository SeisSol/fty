# fty - Fortran To Yaml

It is a small header-only C++ library which is supposed to help people to start converting their legacy Fortran code to C++. The library reads a text file or lists of strings formatted according  to Fortran *NAMELIST I/O* style, and generates a YAML::Node (a hash table) containing retrieved information.

## Requirements
Make sure that you have **yaml-cpp** installed

## Exceptions
The library throws the following exceptions in case of a failure during a convertion:
```
fty::exception::FileException
fty::exception::CriticalTextBlockException
fty::exception::CriticalKeyValueError
```

## Example
```
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
```