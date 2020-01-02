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

#include "verilog/analysis/checkers/module_filename_rule.h"

#include <initializer_list>
#include <string>

#include "gtest/gtest.h"
#include "common/analysis/linter_test_utils.h"
#include "common/analysis/text_structure_linter_test_utils.h"
#include "common/text/symbol.h"
#include "verilog/analysis/verilog_analyzer.h"
#include "verilog/parser/verilog_token_enum.h"

namespace verilog {
namespace analysis {
namespace {

using verible::LintTestCase;
using verible::RunLintTestCases;

// Expected token type of findings.
constexpr int kToken = SymbolIdentifier;

// Test that no violations are found with an empty filename.
TEST(ModuleFilenameRuleTest, BlankFilename) {
  const std::initializer_list<LintTestCase> kTestCases = {
      {""},
      {"module m; endmodule"},
      {"class c; endclass"},
  };
  const std::string filename = "";
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases, filename);
}

// Test that as long as one module matches file name, no violations reported.
TEST(ModuleFilenameRuleTest, ModuleMatchesFilename) {
  const std::initializer_list<LintTestCase> kTestCases = {
      {""},
      {"module m; endmodule"},
      {"module n; endmodule\nmodule m; endmodule"},
      {"module m; endmodule\nmodule n; endmodule"},
  };
  const std::string filename = "/path/to/m.sv";
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases, filename);
}

// Test more unusual file names with multiple dots in them.
TEST(ModuleFilenameRuleTest, ModuleMatchesMultiDotComponentFilename) {
  const std::initializer_list<LintTestCase> kTestCases = {
      {""},
      {"package q; endpackage\n"},
      {"module m; endmodule\n"},
      {"module n; endmodule\nmodule m; endmodule"},
      {"module m; endmodule\nmodule n; endmodule"},
  };
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/m");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/m.v");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/m.sv");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/m.stub.sv");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(
      kTestCases, "/path/to/m.behavioral.model.sv");
}

// Test that some violations are found checked against a filename.
TEST(ModuleFilenameRuleTest, NoModuleMatchesFilenameAbsPath) {
  const std::initializer_list<LintTestCase> kTestCases = {
      {""},
      {"package q; endpackage\n"},
      {"module ", {kToken, "m"}, "; endmodule"},
      {"module m; endmodule\nmodule ", {kToken, "n"}, "; endmodule"},
  };
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/q.sv");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/q.stub.sv");
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases,
                                                        "/path/to/q.m.sv");
}

// Test that some violations are found checked against a filename.
TEST(ModuleFilenameRuleTest, NoModuleMatchesFilenameRelPath) {
  const std::initializer_list<LintTestCase> kTestCases = {
      {""},
      {"module ", {kToken, "m"}, "; endmodule"},
      {"module m; endmodule\nmodule ", {kToken, "n"}, "; endmodule"},
  };
  const std::string filename = "path/to/r.sv";
  RunLintTestCases<VerilogAnalyzer, ModuleFilenameRule>(kTestCases, filename);
}

}  // namespace
}  // namespace analysis
}  // namespace verilog
