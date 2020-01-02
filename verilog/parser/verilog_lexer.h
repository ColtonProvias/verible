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

// VerilogLexer class implements Lexer interface and is implemented in terms
// of verilogFlexLexer that is generated by Flex.

#ifndef VERIBLE_VERILOG_PARSER_VERILOG_LEXER_H__
#define VERIBLE_VERILOG_PARSER_VERILOG_LEXER_H__

// verilog.lex has "%prefix=verilog", meaning the class flex creates is
// verilogFlexLexer. Unfortunately, FlexLexer.h doesn't have proper ifdefs
// around its inclusion, so we have to put a bar around it here.
#include "common/lexer/flex_lexer_adapter.h"
#include "common/text/token_info.h"
#ifndef _FLEXLEXER_H_
#undef yyFlexLexer  // this is how FlexLexer.h says to do things
#define yyFlexLexer verilogFlexLexer
#include <FlexLexer.h>
#endif

#include "absl/strings/string_view.h"

namespace verilog {

class VerilogLexer : public verible::FlexLexerAdapter<verilogFlexLexer> {
  using parent_lexer_type = verible::FlexLexerAdapter<verilogFlexLexer>;

 public:
  explicit VerilogLexer(const absl::string_view code);

  // Main lexing function. Will be defined by Flex.
  int yylex() override;

  // Restart lexer with new input stream.
  void Restart(absl::string_view) override;

  // Returns true if token is invalid.
  bool TokenIsError(const verible::TokenInfo&) const override;

  // Filter predicate that can be used for testing and parsing.
  static bool KeepSyntaxTreeTokens(const verible::TokenInfo&);

 private:
  // These variables are controlled by the lexer code (verilog.lex).

  // for macro call argument lexing
  int balance_;

  // TODO(fangism): This won't be needed.  Remove when appropriate.
  // for keeping track of end of macro-id
  int macro_id_length_;

  // In the MACRO_ARG_UNLEXED state  (verilog.lex), track the number of
  // characters excluding trailing spaces/newlines.
  int macro_arg_length_;
};

}  // namespace verilog

#endif  // VERIBLE_VERILOG_PARSER_VERILOG_LEXER_H__
