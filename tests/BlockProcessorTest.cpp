// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#include "BlockProcessor.hpp"
#include "FtyDataTypes.hpp"
#include "FtyExceptions.hpp"
#include "helper.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <iterator>
#include <list>
#include <yaml-cpp/yaml.h>

using namespace fty;

auto isEqualBlocks(const BlockT& Left, const BlockT& Right) -> bool {
  auto LeftSize = std::distance(Left.first, std::next(Left.second));
  auto RightSize = std::distance(Right.first, std::next(Right.second));
  if (LeftSize != RightSize) {
    std::cout << "LeftSize: " << LeftSize << "; RightSize: " << RightSize << '\n';
    return false;
  }

  auto LeftItr = Left.first;
  auto RightItr = Right.first;
  for (int I = 0; I < LeftSize; ++I, ++LeftItr, ++RightItr) {
    if ((*LeftItr) != (*RightItr)) {
      std::cout << "Left: " << *LeftItr << "; Right: " << *RightItr << ";" << '\n';
      return false;
    }
  }
  return true;
}

TEST(BlockProcessorTest, RemoveEmptyBlocks) {
  BlockFactory Factory;

  Factory.add({{"&Empty"}, {"/"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{"&Empty"}, {"/"}});
  Factory.add({{"&Boundaries"}, {"Order=1 "}, {"BC_fs = 1.5"}, {"BC_of = 1"}, {"/"}});
  Factory.add({{"&Empty"}, {"/"}});

  const StringsT Content = Factory.getContent();
  const std::list<BlockT> Blocks = Factory.getBlocks();

  BlockFactory TestFactory = Factory;
  const StringsT TestContent = TestFactory.getContent();
  std::list<BlockT> TestBlocks = TestFactory.getBlocks();

  fty::BlockProcessor::removeEmptyBlocks(TestBlocks);
  ASSERT_EQ(TestBlocks.size(), 2);
  ASSERT_TRUE(isEqualBlocks(BlockScroll(TestBlocks)[0], BlockScroll(Blocks)[1]));
  ASSERT_TRUE(isEqualBlocks(BlockScroll(TestBlocks)[1], BlockScroll(Blocks)[3]));
}

TEST(BlockProcessorTest, StartsWithCorruptedNextBlock) {
  BlockFactory Factory;

  Factory.add({{"&Empty"}, {"Dummy = 1"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});
  StringsT Content = Factory.getContent();
  auto Begin = Content.begin();
  auto End = Content.end();
  ASSERT_THROW(fty::BlockProcessor::getNextBlock(Begin, End),
               exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EndsWithCorruptedNextBlock) {
  BlockFactory Factory;

  Factory.add({{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}});

  Factory.add({{"&Empty"}, {"Dummy = 1"}, {"/"}});

  StringsT Content = Factory.getContent();
  auto Begin = Content.begin();
  auto End = Content.end();
  ASSERT_THROW(fty::BlockProcessor::getNextBlock(Begin, End),
               exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EndsWithoutTerminator) {
  BlockFactory Factory;

  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{"&Empty"}, {"Dummy = 1"}});

  StringsT Content = Factory.getContent();
  const std::list<BlockT> Blocks = Factory.getBlocks();

  auto Begin = Content.begin();
  auto End = Content.end();
  const BlockT ValidBlock = fty::BlockProcessor::getNextBlock(Begin, End);
  ASSERT_TRUE(isEqualBlocks(ValidBlock, BlockScroll(Blocks)[0]));
  ASSERT_THROW(fty::BlockProcessor::getNextBlock(Begin, End),
               exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EmptyLinesAfterLastBlock) {
  BlockFactory Factory;

  Factory.add({{""}, {"\n"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{""}, {"\n"}});

  StringsT Content = Factory.getContent();
  const std::list<BlockT> Blocks = Factory.getBlocks();

  auto Begin = Content.begin();
  auto End = Content.end();
  const BlockT ValidBlock = fty::BlockProcessor::getNextBlock(Begin, End);
  ASSERT_TRUE(isEqualBlocks(ValidBlock, BlockScroll(Blocks)[1]));
  ASSERT_THROW(fty::BlockProcessor::getNextBlock(Begin, End), exception::TextBlockException);
}
