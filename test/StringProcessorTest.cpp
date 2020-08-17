#include "gtest/gtest.h"
#include "StringProcessor.h"
#include <iostream>
#include <string>

using namespace fty;

TEST(Found, StartWithString) {
  StringProcessor Processor;
  std::string TestString;

  TestString = "&DataInFortranStyle    \n";
  ASSERT_TRUE(Processor.startsWith(TestString, '&'));

  TestString = "  \t   &DataInFortranStyle";
  ASSERT_TRUE(Processor.startsWith(TestString, '&'));
}


TEST(NotFound, StartWithString) {
  StringProcessor Processor;
  std::string TestString;

  TestString = " DataInFortranStyle&    \n";
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));

  TestString = "  \t   DataInFortranStyle";
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));

  TestString = "!   &DataInFortranStyle";
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));
}


TEST(EmptyStrings, StartWithString) {
  StringProcessor Processor;
  std::string TestString{"\n"};
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));

  TestString = "\n\n\n";
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));

  TestString = "";
  ASSERT_FALSE(Processor.startsWith(TestString, '&'));
}


TEST(Found, removeComments) {
  StringsT Content;
  Content.push_back("\n");
  Content.push_back("   ");
  Content.push_back("!  Comments ");
Content.push_back("!  Comments ");


  StringProcessor Processor;
  std::string TestString;

  TestString = "&DataInFortranStyle    \n";
  ASSERT_TRUE(Processor.startsWith(TestString, '&'));

  TestString = "  \t   &DataInFortranStyle";
  ASSERT_TRUE(Processor.startsWith(TestString, '&'));
}