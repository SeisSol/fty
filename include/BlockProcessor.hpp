#ifndef FTY_CONVERTER_BLOCKPROCESSOR_HPP
#define FTY_CONVERTER_BLOCKPROCESSOR_HPP

#include "FtyInternals.hpp"
#include "StringProcessor.h"
#include <iostream>

namespace fty {

class BlockProcessor {
public:
  /**
   * Finds a block given the beginning, end, comment symbols of a block. 'Bigin' iterator will be
   * modified which will point to the next line after the end block symbol
   *
   * @param Begin iterator which points to the first line to consider (mutable)
   * @param End iterator which points to the last line to consider
   * @param ControlBlockChars a tuple of 3 characters which determine the beginning, end, comment of
   * a block
   * @return a list of string which contains a block
   */
  BlockT getNextBlock(StringsT::iterator &CurrentItr, const StringsT::iterator End) {
    BlockT Block;
    const char BlockBeginChar = '&';
    const char BlockEndChar = '/';

    if (CurrentItr == End) {
      throw exception::TextBlockException("given an empty range");
    }

    // skip everything above which is not a block
    while ((CurrentItr != End) && (!m_StringProcessor.startsWith(*CurrentItr, BlockBeginChar))) {
      ++CurrentItr;
    }
    if (CurrentItr == End) {
      throw exception::TextBlockException("a range doesn't contain a Fortran block");
    }
    // The beginning (header) of a block is found if nothing is thrown till here

    // record the found header and advance iterator to the next position
    Block.first = CurrentItr++;

    bool IsBlockEndFound = false;
    bool IsNextBlockFound = false;
    for (; CurrentItr != End; ++CurrentItr) {
      IsBlockEndFound = m_StringProcessor.startsWith(*CurrentItr, BlockEndChar);
      IsNextBlockFound = m_StringProcessor.startsWith(*CurrentItr, BlockBeginChar);

      if (IsBlockEndFound or IsNextBlockFound)
        break;
    }

    if (IsNextBlockFound) {
      throw exception::CriticalTextBlockException("found an incomplete block: " + *Block.first);
    }

    if (!IsBlockEndFound) {
      throw exception::CriticalTextBlockException("cannot find the end of block: " + *Block.first);
    }

    // If we reach this point It means that the 'Begin' iterator points to '/'.
    Block.second = CurrentItr;

    // Advance to the next if it is not the end
    if (CurrentItr != End)
      ++CurrentItr;

    return Block;
  }

  void removeEmptyBlocks(std::list<BlockT> &Blocks) {

    // NOTE: header + tail + at least one field
    const std::list<BlockT>::iterator::difference_type MIN_NUM_STRINGS = 2;

    std::vector<std::list<BlockT>::iterator> Deletees;
    for (auto Itr = Blocks.begin(); Itr != Blocks.end(); ++Itr) {
      if (std::distance(Itr->first, Itr->second) < MIN_NUM_STRINGS) {
        Deletees.push_back(Itr);
      }
    }

    for (auto &Deletee : Deletees) {
      Blocks.erase(Deletee);
    }
  }

private:
  StringProcessor m_StringProcessor;
};
} // namespace fty

#endif // FTY_CONVERTER_BLOCKPROCESSOR_HPP
