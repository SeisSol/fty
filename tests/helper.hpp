#ifndef FTY_CONVERTER_HELPER_HPP
#define FTY_CONVERTER_HELPER_HPP

#include "FtyDataTypes.hpp"
#include <cassert>
#include <type_traits>

using namespace fty;
BlockT make_block(StringsT& Content);


template <typename ListT>
class Scroll {
public:
  Scroll(const ListT& Content) : m_Content(Content) {};
  typename ListT::value_type operator[](int Index) {
    assert(Index < m_Content.size() && "Index is out of the range");
    return *(std::next(m_Content.begin(), Index));
  }

private:
  const ListT& m_Content;
};

using StringsScroll = Scroll<StringsT>;
using BlockScroll = Scroll<std::list<BlockT>>;


class BlockFactory {
public:
  BlockFactory() = default;
  BlockFactory(const BlockFactory& Other) {
    for (const auto& Block: Other.m_Blocks) {
      m_Content.insert(m_Content.end(), Block.first, std::next(Block.second));
      auto Head = std::prev(m_Content.end(), std::distance(Block.first, std::next(Block.second)));
      auto Tail = std::prev(m_Content.end());
      m_Blocks.push_back(std::make_pair(Head, Tail));
    }
  }

  void add(StringsT LocalContent) {
    m_Content.insert(m_Content.end(), LocalContent.begin(), LocalContent.end());
    auto Head = std::prev(m_Content.end(), LocalContent.size());
    auto Tail = std::prev(m_Content.end());
    m_Blocks.push_back(std::make_pair(Head, Tail));
  }

  StringsT getContent() {
    return m_Content;
  }

  std::list<BlockT> getBlocks() {
    return m_Blocks;
  }

private:
  StringsT m_Content;
  std::list<BlockT> m_Blocks;
};


#endif //FTY_CONVERTER_HELPER_HPP
