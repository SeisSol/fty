#ifndef PARSER_BLOCKFINDER_T_H
#define PARSER_BLOCKFINDER_T_H


#include "FtyBlockParsing.hpp"
#include <string>
#include <cxxtest/TestSuite.h>

class BlockFinder : public CxxTest::TestSuite {
public:
  void testCorrectBlocks() {
    {
      StringsT Content;
      Content.push_back("&AbortCriteria\n");
      Content.push_back("! &Comment\n");
      Content.push_back("\n");
      Content.push_back("EndTime = 10.0\n");
      Content.push_back("/\n");
      Content.push_back("\n");

      StringsT::iterator Begin = Content.begin();
      StringsT::iterator End = Content.end();

      StringsT Block = getNextBlock(Begin, End, FortranControlBlock);

      const int STRING_EQUAL = 0;
      // Begin should move
      TS_ASSERT_EQUALS(Begin == End, true);
      //TS_ASSERT_EQUALS((*(Begin)).compare("/\n"), STRING_EQUAL);

      // 3 because the comment and the last '/' were deleted
      TS_ASSERT_EQUALS(Block.size(), 3);

      StringsT::iterator BlockBegin = Block.begin();
      TS_ASSERT_EQUALS((*(BlockBegin++)).compare("&AbortCriteria\n"), STRING_EQUAL);
      TS_ASSERT_EQUALS((*(BlockBegin++)).compare("\n"), STRING_EQUAL);
      TS_ASSERT_EQUALS((*(BlockBegin)).compare("EndTime = 10.0\n"), STRING_EQUAL);
    }
  }

  void testCorruptedBlocks() {
    {
      StringsT Content;
      Content.push_back("\n");
      Content.push_back("\n");

      StringsT::iterator Begin = Content.begin();
      StringsT::iterator End = Content.end();

      TS_ASSERT_THROWS( getNextBlock(Begin, End, FortranControlBlock), TextBlockException);
    }

    {
      // Content doesn't have the closing '/' character. It should lead to the critical error
      StringsT Content;
      Content.push_back("&AbortCriteria\n");
      Content.push_back("! &Comment\n");
      Content.push_back("\n");
      Content.push_back("EndTime = 10.0\n");

      StringsT::iterator Begin = Content.begin();
      StringsT::iterator End = Content.end();

      TS_ASSERT_THROWS( getNextBlock(Begin, End, FortranControlBlock), CriticalTextBlockException);
    }

    {
      // Content doesn't have the closing '/' character. It should lead to the critical error
      StringsT Content;
      Content.push_back("&Debugging\n");
      Content.push_back("\n");

      StringsT::iterator Begin = Content.begin();
      StringsT::iterator End = Content.end();

      TS_ASSERT_THROWS( getNextBlock(Begin, End, FortranControlBlock), CriticalTextBlockException);
    }
  }

};

#endif //PARSER_BLOCKFINDER_T_H
