#include "helper.hpp"
#include "BlockParser.hpp"
#include "FtyPolicies.hpp"
#include "gtest/gtest.h"
#include <yaml-cpp/yaml.h>
#include <string>

using namespace fty;

class PolicyTests : public ::testing::Test {
protected:
  void SetUp() override {
    m_Content.push_back("&Discretization\n");
    m_Content.push_back("Order = 4\n");
    m_Content.push_back(" Material = 1 \n");
    m_Content.push_back(" Cfl = 0.5 ");
    m_Content.push_back("/");

    m_TestContent = m_Content;
  }
  StringsT m_Content;
  StringsT m_TestContent;
};


TEST_F(PolicyTests, LowercaseTest) {
  BlockT Block = make_block(m_TestContent);
  BlockParser<As_lowercase> Processor;

  std::string Header = Processor.getHeader(Block);
  ASSERT_STREQ(Header.c_str(), "discretization");

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_STREQ(Node["order"].as<std::string>().c_str(), "4");
  ASSERT_STREQ(Node["material"].as<std::string>().c_str(), "1");
  ASSERT_STREQ(Node["cfl"].as<std::string>().c_str(), "0.5");
}


TEST_F(PolicyTests, UppercaseTest) {
  BlockT Block = make_block(m_TestContent);
  BlockParser<As_uppercase> Processor;

  std::string Header = Processor.getHeader(Block);
  ASSERT_STREQ(Header.c_str(), "DISCRETIZATION");

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_STREQ(Node["ORDER"].as<std::string>().c_str(), "4");
  ASSERT_STREQ(Node["MATERIAL"].as<std::string>().c_str(), "1");
  ASSERT_STREQ(Node["CFL"].as<std::string>().c_str(), "0.5");
}


TEST_F(PolicyTests, DefaultPolicyTest) {
  BlockT Block = make_block(m_TestContent);
  BlockParser<As_original> Processor;

  std::string Header = Processor.getHeader(Block);
  ASSERT_STREQ(Header.c_str(), "Discretization");

  YAML::Node Node = Processor.getFields(Block);
  ASSERT_STREQ(Node["Order"].as<std::string>().c_str(), "4");
  ASSERT_STREQ(Node["Material"].as<std::string>().c_str(), "1");
  ASSERT_STREQ(Node["Cfl"].as<std::string>().c_str(), "0.5");
}