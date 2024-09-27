// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#include "StringProcessor.hpp"
#include "helper.hpp"
#include "gtest/gtest.h"
#include <string>

using namespace fty;

TEST(StartWithString, Found) {
  std::string TestString;

  TestString = "&DataInFortranStyle    \n";
  ASSERT_TRUE(fty::StringProcessor::startsWith(TestString, '&'));

  TestString = "  \t   &DataInFortranStyle";
  ASSERT_TRUE(fty::StringProcessor::startsWith(TestString, '&'));
}

TEST(StartWithString, NotFound) {
  std::string TestString;

  TestString = " DataInFortranStyle&    \n";
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));

  TestString = "  \t   DataInFortranStyle";
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));

  TestString = "!   &DataInFortranStyle";
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));
}

TEST(StartWithString, EmptyStrings) {
  std::string TestString{"\n"};
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));

  TestString = "\n\n\n";
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));

  TestString = "";
  ASSERT_FALSE(fty::StringProcessor::startsWith(TestString, '&'));
}

class CommentsAndEmptyLinesTest : public ::testing::Test {
  protected:
  void SetUp() override {
    m_Content.emplace_back("\n");
    m_Content.emplace_back("   ");
    m_Content.emplace_back("   \n");
    m_Content.emplace_back("\n   ");
    m_Content.emplace_back(" !  Comments ");
    m_Content.emplace_back("   \r\n");
    m_Content.emplace_back("value1 = 3.14 !  Comments ");
    m_Content.emplace_back(" value2 = 2.71 ! Comments ! Comment\n");
    m_Content.emplace_back("value3 = 9.81! Comments ! Comment\n");
    m_Content.emplace_back("");

    m_TestContent = m_Content;
  }

  StringsT m_Content;
  StringsT m_TestContent;
};

TEST_F(CommentsAndEmptyLinesTest, removeEmptyLines) {

  fty::StringProcessor::removeEmptyLines(m_TestContent);

  ASSERT_EQ(m_TestContent.size(), 4);
  ASSERT_STREQ(StringsScroll(m_TestContent)[0].c_str(), StringsScroll(m_Content)[4].c_str());
  ASSERT_STREQ(StringsScroll(m_TestContent)[1].c_str(), StringsScroll(m_Content)[6].c_str());
  ASSERT_STREQ(StringsScroll(m_TestContent)[2].c_str(), StringsScroll(m_Content)[7].c_str());
  ASSERT_STREQ(StringsScroll(m_TestContent)[3].c_str(), StringsScroll(m_Content)[8].c_str());
}

TEST_F(CommentsAndEmptyLinesTest, removeComments) {

  fty::StringProcessor::removeComments(m_TestContent);

  // removeComments just trims comments
  ASSERT_EQ(m_TestContent.size(), m_Content.size());
  ASSERT_STREQ(StringsScroll(m_TestContent)[4].c_str(), " "); // (" !  Comments ")
  ASSERT_STREQ(StringsScroll(m_TestContent)[6].c_str(), "value1 = 3.14 ");
  ASSERT_STREQ(StringsScroll(m_TestContent)[7].c_str(), " value2 = 2.71 ");
  ASSERT_STREQ(StringsScroll(m_TestContent)[8].c_str(), "value3 = 9.81");
}

TEST_F(CommentsAndEmptyLinesTest, CombinedTest) {

  fty::StringProcessor::removeComments(m_TestContent);
  fty::StringProcessor::removeEmptyLines(m_TestContent);

  // removeComments just trims comments
  ASSERT_EQ(m_TestContent.size(), 3);
  auto Itr = m_TestContent.begin();
  ASSERT_STREQ((*Itr).c_str(), "value1 = 3.14 ");
  ASSERT_STREQ((*(++Itr)).c_str(), " value2 = 2.71 ");
  ASSERT_STREQ((*(++Itr)).c_str(), "value3 = 9.81");
}
