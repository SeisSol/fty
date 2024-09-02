// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#ifndef FTY_CONVERTER_HELPER_HPP
#define FTY_CONVERTER_HELPER_HPP

#include "FtyDataTypes.hpp"
#include <cassert>
#include <type_traits>

using namespace fty;
auto make_block(StringsT &Content) -> BlockT;

template <typename ListT> class Scroll {
public:
  Scroll(const ListT &Content) : m_Content(Content){};
  auto operator[](size_t Index) -> typename ListT::value_type {
    assert(Index < m_Content.size() && "Index is out of the range");
    return *(std::next(m_Content.begin(), Index));
  }

private:
  const ListT &m_Content;
};

using StringsScroll = Scroll<StringsT>;
using BlockScroll = Scroll<std::list<BlockT>>;

class BlockFactory {
public:
  BlockFactory() = default;
  BlockFactory(const BlockFactory &Other) {
    for (const auto &Block : Other.m_Blocks) {
      m_Content.insert(m_Content.end(), Block.first, std::next(Block.second));
      auto Head = std::prev(m_Content.end(), std::distance(Block.first, std::next(Block.second)));
      auto Tail = std::prev(m_Content.end());
      m_Blocks.emplace_back(Head, Tail);
    }
  }

  auto operator=(const BlockFactory&) -> BlockFactory& = delete; // TODO
  BlockFactory(BlockFactory&&) = delete; // TODO
  auto operator=(BlockFactory&&) -> BlockFactory& = delete; // TODO
  ~BlockFactory() = default;

  void add(StringsT LocalContent) {
    m_Content.insert(m_Content.end(), LocalContent.begin(), LocalContent.end());
    auto Head = std::prev(m_Content.end(), static_cast<ssize_t>(LocalContent.size()));
    auto Tail = std::prev(m_Content.end());
    m_Blocks.emplace_back(Head, Tail);
  }

  auto getContent() -> StringsT { return m_Content; }

  auto getBlocks() -> std::list<BlockT> { return m_Blocks; }

private:
  StringsT m_Content;
  std::list<BlockT> m_Blocks;
};

#endif // FTY_CONVERTER_HELPER_HPP
