#include "BlockParser.hpp"
#include "FtyPolicies.hpp"
#include "helper.hpp"
#include "gtest/gtest.h"
#include <string>
#include <yaml-cpp/yaml.h>

using namespace fty;

TEST(HeaderTest, HeaderWithWhitespaces) {
  BlockParser<AsOriginal> Processor;
  StringsT Content{{"  &  Discretization  "}, {"Order = 1"}};
  BlockT Block = make_block(Content);
  std::string Header = Processor.getHeader(Block);
  ASSERT_STREQ(Header.c_str(), "Discretization");
}

TEST(HeaderTest, HeaderWithComment) {
  BlockParser<AsOriginal> Processor;
  StringsT Content{{"&Discretization  ! Comment \n"}, {"Order = 1"}};
  BlockT Block = make_block(Content);
  std::string Header = Processor.getHeader(Block);
  ASSERT_STREQ(Header.c_str(), "Discretization");
}

TEST(InvalidHeaderTest, InvalidHeader) {
  BlockParser<AsOriginal> Processor;
  // forgotten '&' symbol
  StringsT Content{{"Discretization"}, {"Order = 1"}};
  BlockT Block = make_block(Content);

  ASSERT_THROW(Processor.getHeader(Block), exception::CriticalTextBlockException);
}

TEST(InvalidBlockFieldTest, EmptyBlock) {
  BlockParser<AsOriginal> Processor;
  // forgotten '&' symbol
  StringsT Content{{"&Discretization"}, {"/"}};
  BlockT Block = make_block(Content);

  ASSERT_THROW(Processor.getFields(Block), exception::CriticalTextBlockException);
}

TEST(InvalidBlockFieldTest, RepeatedFields) {
  BlockParser<AsOriginal> Processor;
  // forgotten '&' symbol
  StringsT Content{{"&Discretization"}, {"Order = 1"}, {"Order = 1"}, {"/"}};
  BlockT Block = make_block(Content);

  ASSERT_THROW(Processor.getFields(Block), exception::CriticalKeyValueError);
}

TEST(InvalidBlockFieldTest, InvalidField) {
  BlockParser<AsOriginal> Processor;
  // forgotten '=' symbol
  StringsT Content{{"&Discretization"}, {"Order  1"}, {"/"}};
  BlockT Block = make_block(Content);

  ASSERT_THROW(Processor.getFields(Block), exception::CriticalTextBlockException);
}

TEST(CorrectBlockTest, CorrectBlock) {
  BlockParser<AsLowercase> Processor;
  // forgotten '=' symbol
  StringsT Content{{"&Discretization"},
                   {"Order=1 "},
                   {"Cfl = 0.5  \n"},
                   {"OutputFile = \"output/tpv5\'"},
                   {"InputFile = \'in_put-file/tpv5.txt\'  \n"},
                   {"MASK1 =   0  1 0 1   0 1 "},
                   {"MASK2 =1 0 1 0 0 1"},
                   {"/"}};
  BlockT Block = make_block(Content);

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_STREQ(Node["order"].as<std::string>().c_str(), "1");
  ASSERT_STREQ(Node["cfl"].as<std::string>().c_str(), "0.5");
  ASSERT_STREQ(Node["outputfile"].as<std::string>().c_str(), "output/tpv5");
  ASSERT_STREQ(Node["inputfile"].as<std::string>().c_str(), "in_put-file/tpv5.txt");
  ASSERT_STREQ(Node["mask1"].as<std::string>().c_str(), "0  1 0 1   0 1");
  ASSERT_STREQ(Node["mask2"].as<std::string>().c_str(), "1 0 1 0 0 1");
}

TEST(FloatingPointField, StandardNotaion) {
  BlockParser<AsOriginal> Processor;
  StringsT Content{{"&Block1"},
                   {"Epsilon1 = 1.01e-10"},
                   {"Epsilon2 = 1.01e+10"},
                   {"Epsilon3 = 1.01E-10"},
                   {"Epsilon4 = 1.01E+10"},
                   {"/"}};
  BlockT Block = make_block(Content);

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_DOUBLE_EQ(Node["Epsilon1"].as<double>(), 1.01e-10);
  ASSERT_FLOAT_EQ(Node["Epsilon2"].as<float>(), 1.01e+10f);
  ASSERT_DOUBLE_EQ(Node["Epsilon3"].as<double>(), 1.01E-10);
  ASSERT_FLOAT_EQ(Node["Epsilon4"].as<float>(), 1.01E+10f);
}

TEST(FloatingPointField, FortranNotaion) {
  BlockParser<AsOriginal> Processor;
  StringsT Content{{"&Block1"},
                   {"Epsilon1=1.01d-10"},
                   {"Epsilon2=1.01d+10"},
                   {"Epsilon3=1.01D-10"},
                   {"Epsilon4=1.01D+10"},
                   {"/"}};
  BlockT Block = make_block(Content);

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_DOUBLE_EQ(Node["Epsilon1"].as<double>(), 1.01e-10);
  ASSERT_FLOAT_EQ(Node["Epsilon2"].as<float>(), 1.01e+10f);
  ASSERT_DOUBLE_EQ(Node["Epsilon3"].as<double>(), 1.01e-10);
  ASSERT_FLOAT_EQ(Node["Epsilon4"].as<float>(), 1.01e+10f);
}
