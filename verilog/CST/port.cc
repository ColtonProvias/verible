// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "verilog/CST/port.h"

#include <vector>

#include "common/analysis/matcher/matcher.h"
#include "common/analysis/matcher/matcher_builders.h"
#include "common/analysis/syntax_tree_search.h"
#include "common/text/concrete_syntax_leaf.h"
#include "common/text/concrete_syntax_tree.h"
#include "common/text/symbol.h"
#include "common/text/tree_utils.h"
#include "common/util/logging.h"
#include "verilog/CST/identifier.h"
#include "verilog/CST/verilog_matchers.h"  // pragma IWYU: keep

namespace verilog {

using verible::Symbol;
using verible::SyntaxTreeLeaf;

std::vector<verible::TreeSearchMatch> FindAllModulePortDeclarations(
    const Symbol& root) {
  return SearchSyntaxTree(root, NodekPortDeclaration());
}

std::vector<verible::TreeSearchMatch> FindAllTaskFunctionPortDeclarations(
    const Symbol& root) {
  return SearchSyntaxTree(root, NodekPortItem());
}

const SyntaxTreeLeaf* GetIdentifierFromModulePortDeclaration(
    const Symbol& symbol) {
  const auto* identifier_symbol =
      verible::GetSubtreeAsSymbol(symbol, NodeEnum::kPortDeclaration, 3);
  return AutoUnwrapIdentifier(*ABSL_DIE_IF_NULL(identifier_symbol));
}

static const verible::SyntaxTreeNode&
GetTypeIdDimensionsFromTaskFunctionPortItem(const Symbol& symbol) {
  return verible::GetSubtreeAsNode(
      symbol, NodeEnum::kPortItem, 1,
      NodeEnum::kDataTypeImplicitBasicIdDimensions);
}

const Symbol* GetTypeOfTaskFunctionPortItem(const verible::Symbol& symbol) {
  const auto& type_id_dimensions =
      GetTypeIdDimensionsFromTaskFunctionPortItem(symbol);
  return &verible::GetSubtreeAsNode(
      type_id_dimensions, NodeEnum::kDataTypeImplicitBasicIdDimensions, 0,
      NodeEnum::kDataType);
}

const SyntaxTreeLeaf* GetIdentifierFromTaskFunctionPortItem(
    const verible::Symbol& symbol) {
  const auto& type_id_dimensions =
      GetTypeIdDimensionsFromTaskFunctionPortItem(symbol);
  return AutoUnwrapIdentifier(*ABSL_DIE_IF_NULL(type_id_dimensions[1].get()));
}

}  // namespace verilog
