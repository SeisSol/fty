#ifndef PARSER_BLOCKREMOVER_T_H
#define PARSER_BLOCKREMOVER_T_H

#include "FtyBlockParsing.hpp"
#include <string>
#include <cxxtest/TestSuite.h>

class BlockFilters : public CxxTest::TestSuite {
public:
  void testEmptyBlockRemover() {
    /*
    {

      // Content doesn't have the closing '/' character. It should lead to the critical error
      std::vector<StringsT> Blocks;
      {
        StringsT Block;
        Block.push_back("&Debugging\n");
        Block.push_back("\n");
        Blocks.push_back(Block);
      }

      removeEmptyBlocks(Blocks);

      for (auto& Block: Blocks) {
        std::cout << std::endl << std::string(80, '-') << std::endl;
        std::cout << Block;
      }

      TS_ASSERT_EQUALS(Blocks.size(), 0);

    }
    */
    {
      // Content doesn't have the closing '/' character. It should lead to the critical error
      std::vector<StringsT> Blocks;
      {
        StringsT Block;
        Block.push_back("&SourceType\n");
        Blocks.push_back(Block);
      }

      {
        StringsT Block;
        Block.push_back("&DynamicRupture\n");
        Blocks.push_back(Block);
      }

      {
        StringsT Block;
        Block.push_back("&SourceType\n");
        Blocks.push_back(Block);
      }

      /*
      removeEmptyBlocks(Blocks);

      for (auto& Block: Blocks) {
        std::cout << std::endl << std::string(80, '-') << std::endl;
        std::cout << Block;
      }

      TS_ASSERT_EQUALS(Blocks.size(), 0);
      */
    }
  }

  void testEmptyBlockRemover1() {

  }
};

#endif //PARSER_BLOCKREMOVER_T_H
