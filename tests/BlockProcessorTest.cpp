// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#include "BlockProcessor.hpp"
#include "FtyDataTypes.hpp"
#include "FtyExceptions.hpp"
#include "helper.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace fty;

bool isEqualBlocks(const BlockT& Left, const BlockT& Right) {
  auto LeftSize = std::distance(Left.first, std::next(Left.second));
  auto RightSize = std::distance(Right.first, std::next(Right.second));
  if (LeftSize != RightSize) {
    std::cout << "LeftSize: " << LeftSize << "; RightSize: " << RightSize << std::endl;
    return false;
  }

  auto LeftItr = Left.first;
  auto RightItr = Right.first;
  for (int I = 0; I < LeftSize; ++I, ++LeftItr, ++RightItr) {
    if ((*LeftItr) != (*RightItr)) {
      std::cout << "Left: " << *LeftItr << "; Right: " << *RightItr << ";" << std::endl;
      return false;
    }
  }
  return true;
}

TEST(BlockProcessorTest, RemoveEmptyBlocks) {
  BlockProcessor Processor;
  BlockFactory Factory;

  Factory.add({{"&Empty"}, {"/"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{"&Empty"}, {"/"}});
  Factory.add({{"&Boundaries"}, {"Order=1 "}, {"BC_fs = 1.5"}, {"BC_of = 1"}, {"/"}});
  Factory.add({{"&Empty"}, {"/"}});

  const StringsT Content = Factory.getContent();
  std::list<BlockT> Blocks = Factory.getBlocks();

  BlockFactory TestFactory = Factory;
  const StringsT TestContent = TestFactory.getContent();
  std::list<BlockT> TestBlocks = TestFactory.getBlocks();

  Processor.removeEmptyBlocks(TestBlocks);
  ASSERT_EQ(TestBlocks.size(), 2);
  ASSERT_TRUE(isEqualBlocks(BlockScroll(TestBlocks)[0], BlockScroll(Blocks)[1]));
  ASSERT_TRUE(isEqualBlocks(BlockScroll(TestBlocks)[1], BlockScroll(Blocks)[3]));
}

TEST(BlockProcessorTest, StartsWithCorruptedNextBlock) {
  BlockProcessor Processor;
  BlockFactory Factory;

  Factory.add({{"&Empty"}, {"Dummy = 1"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});
  StringsT Content = Factory.getContent();
  auto Begin = Content.begin();
  auto End = Content.end();
  ASSERT_THROW(Processor.getNextBlock(Begin, End), exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EndsWithCorruptedNextBlock) {
  BlockProcessor Processor;
  BlockFactory Factory;

  Factory.add({{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}});

  Factory.add({{"&Empty"}, {"Dummy = 1"}, {"/"}});

  StringsT Content = Factory.getContent();
  auto Begin = Content.begin();
  auto End = Content.end();
  ASSERT_THROW(Processor.getNextBlock(Begin, End), exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EndsWithoutTerminator) {
  BlockProcessor Processor;
  BlockFactory Factory;

  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{"&Empty"}, {"Dummy = 1"}});

  StringsT Content = Factory.getContent();
  std::list<BlockT> Blocks = Factory.getBlocks();

  auto Begin = Content.begin();
  auto End = Content.end();
  const BlockT ValidBlock = Processor.getNextBlock(Begin, End);
  ASSERT_TRUE(isEqualBlocks(ValidBlock, BlockScroll(Blocks)[0]));
  ASSERT_THROW(Processor.getNextBlock(Begin, End), exception::CriticalTextBlockException);
}

TEST(BlockProcessorTest, EmptyLinesAfterLastBlock) {
  BlockProcessor Processor;
  BlockFactory Factory;

  Factory.add({{""}, {"\n"}});
  Factory.add(
      {{"&Discretization"}, {"Order=1 "}, {"Cfl = 0.5  \n"}, {"MASK2 =1 0 1 0 0 1"}, {"/"}});

  Factory.add({{""}, {"\n"}});

  StringsT Content = Factory.getContent();
  std::list<BlockT> Blocks = Factory.getBlocks();

  auto Begin = Content.begin();
  auto End = Content.end();
  const BlockT ValidBlock = Processor.getNextBlock(Begin, End);
  ASSERT_TRUE(isEqualBlocks(ValidBlock, BlockScroll(Blocks)[1]));
  ASSERT_THROW(Processor.getNextBlock(Begin, End), exception::TextBlockException);
}
