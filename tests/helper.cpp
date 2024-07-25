// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#include "helper.hpp"

BlockT make_block(StringsT& Content) { return std::make_pair(Content.begin(), --Content.end()); }