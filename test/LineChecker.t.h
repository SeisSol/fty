#ifndef PARSER_LINECHECKER_T_H
#define PARSER_LINECHECKER_T_H

#include "FtyBlockParsing.hpp"
#include <string>
#include <cxxtest/TestSuite.h>

class LineChecker : public CxxTest::TestSuite {
public:
  void testStartWithFound() {
    {
      std::string TestString{"&DataInFortranStyle    \n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), true);
    }
    {
      std::string TestString{"   &DataInFortranStyle    \n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), true);
    }
  }

  void testStartWithNotFound() {
    {
      std::string TestString{"!&DataInFortranStyle    \n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
    {
      std::string TestString{"  DataInFortranStyle&    \n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
    {
      std::string TestString{"!   &DataInFortranStyle    \n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
    {
      std::string TestString{"\n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
    {
      std::string TestString{"\n\n\n"};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
    {
      std::string TestString{""};
      TS_ASSERT_EQUALS(startsWith(TestString, '&'), false);
    }
  }
};

#endif //PARSER_LINECHECKER_T_H
