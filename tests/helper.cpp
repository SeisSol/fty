// SPDX-FileCopyrightText: 2020-2023 Ravil Dorozhinskii
//
// SPDX-License-Identifier: MIT

#include "helper.hpp"
#include "FtyDataTypes.hpp"
#include <utility>

BlockT make_block(StringsT& Content) { return std::make_pair(Content.begin(), --Content.end()); }
