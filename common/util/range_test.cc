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

#include "common/util/range.h"

#include <iterator>
#include <string>

#include "gtest/gtest.h"
#include "absl/strings/string_view.h"

namespace verible {
namespace {

// Test that IsSubRange matches same empty string.
TEST(IsSubRangeTest, SameEmptyString) {
  const absl::string_view substring = "";
  EXPECT_TRUE(IsSubRange(substring, substring));
}

// Test that IsSubRange matches same nonempty string.
TEST(IsSubRangeTest, SameNonEmptyString) {
  const absl::string_view substring = "nonempty";
  EXPECT_TRUE(IsSubRange(substring, substring));
}

// Test that IsSubRange is false on completely different string_views.
TEST(IsSubRangeTest, DifferentStringViews) {
  const absl::string_view a = "twiddle-dee";
  const absl::string_view b = "twiddle-dum";
  EXPECT_FALSE(IsSubRange(a, b));
  EXPECT_FALSE(IsSubRange(b, a));
}

// Test that IsSubRange detects non-overlapping string locations.
TEST(IsSubRangeTest, IdenticalSeparateStrings) {
  const std::string superstring = "a";
  const std::string substring = "a";
  EXPECT_FALSE(IsSubRange(substring, superstring));
  EXPECT_FALSE(IsSubRange(superstring, substring));
}

// Test that IsSubRange matches sub-string_view.
TEST(IsSubRangeTest, SubStringView) {
  const absl::string_view superstring = "not-empty";
  EXPECT_TRUE(IsSubRange(superstring.substr(0, 0), superstring));  // empty
  EXPECT_TRUE(IsSubRange(superstring.substr(3, 0), superstring));  // empty
  EXPECT_TRUE(IsSubRange(superstring.substr(0), superstring));
  EXPECT_TRUE(IsSubRange(superstring, superstring.substr(0)));
  EXPECT_TRUE(IsSubRange(superstring.substr(1), superstring));
  EXPECT_TRUE(IsSubRange(superstring.substr(1, 3), superstring));
}

// Test that IsSubRange is false on superstring views (converse).
TEST(IsSubRangeTest, SuperStringView) {
  const absl::string_view superstring = "also-nonempty";
  EXPECT_FALSE(IsSubRange(superstring, superstring.substr(1)));
  EXPECT_FALSE(IsSubRange(superstring, superstring.substr(1, 3)));
}

// Test that IsSubRange works on substring ranges.
TEST(IsSubRangeTest, DerivedSubStringView) {
  const absl::string_view str = "qwertyuiop";
  EXPECT_FALSE(IsSubRange(str.substr(0, 0), str.substr(1, 0)));  // empty
  EXPECT_FALSE(IsSubRange(str.substr(1, 0), str.substr(0, 0)));  // empty
  EXPECT_TRUE(IsSubRange(str.substr(1, 0), str.substr(0, 1)));
  EXPECT_TRUE(IsSubRange(str.substr(1, 1), str.substr(1)));
  EXPECT_TRUE(IsSubRange(str.substr(1, 1), str.substr(1, 1)));
  EXPECT_FALSE(IsSubRange(str.substr(1, 2), str.substr(3, 2)));  // abutting
  EXPECT_FALSE(IsSubRange(str.substr(3, 2), str.substr(1, 2)));  // abutting
  EXPECT_FALSE(IsSubRange(str.substr(1, 2), str.substr(5, 2)));  // disjoint
  EXPECT_FALSE(IsSubRange(str.substr(5, 2), str.substr(1, 2)));  // disjoint
  EXPECT_FALSE(IsSubRange(str.substr(1, 4), str.substr(3, 4)));  // partial
  EXPECT_FALSE(IsSubRange(str.substr(3, 4), str.substr(1, 4)));  // partial
}

// Test that BoundsEqual matches same empty string.
TEST(BoundsEqualTest, SameEmptyString) {
  const absl::string_view substring = "";
  EXPECT_TRUE(BoundsEqual(substring, substring));
}

// Test that BoundsEqual matches same nonempty string.
TEST(BoundsEqualTest, SameNonEmptyString) {
  const absl::string_view substring = "nonempty";
  EXPECT_TRUE(BoundsEqual(substring, substring));
}

// Test that BoundsEqual is false on completely different string_views.
TEST(BoundsEqualTest, DifferentStringViews) {
  const absl::string_view a = "twiddle-dee";
  const absl::string_view b = "twiddle-dum";
  EXPECT_FALSE(BoundsEqual(a, b));
  EXPECT_FALSE(BoundsEqual(b, a));
}

// Test that BoundsEqual is false on non-overlapping string locations.
TEST(BoundsEqualTest, IdenticalSeparateStrings) {
  const std::string superstring = "a";
  const std::string substring = "a";
  EXPECT_FALSE(BoundsEqual(substring, superstring));
  EXPECT_FALSE(BoundsEqual(superstring, substring));
}

// Test that BoundsEqual matches sub-string_view.
TEST(BoundsEqualTest, SubStringView) {
  const absl::string_view superstring = "not-empty";
  EXPECT_FALSE(BoundsEqual(superstring.substr(0, 0), superstring));  // empty
  EXPECT_FALSE(BoundsEqual(superstring.substr(3, 0), superstring));  // empty
  EXPECT_TRUE(BoundsEqual(superstring.substr(0), superstring));
  EXPECT_TRUE(BoundsEqual(superstring, superstring.substr(0)));
  EXPECT_FALSE(BoundsEqual(superstring.substr(1), superstring));
  EXPECT_FALSE(BoundsEqual(superstring.substr(1, 3), superstring));
}

// Test that BoundsEqual is false on superstring views (converse).
TEST(BoundsEqualTest, SuperStringView) {
  const absl::string_view superstring = "also-nonempty";
  EXPECT_FALSE(BoundsEqual(superstring, superstring.substr(1)));
  EXPECT_FALSE(BoundsEqual(superstring, superstring.substr(1, 3)));
}

// Test that BoundsEqual works on substring ranges.
TEST(BoundsEqualTest, DerivedSubStringView) {
  const absl::string_view str = "qwertyuiop";
  EXPECT_FALSE(BoundsEqual(str.substr(0, 0), str.substr(1, 0)));  // empty
  EXPECT_FALSE(BoundsEqual(str.substr(1, 0), str.substr(0, 0)));  // empty
  EXPECT_FALSE(BoundsEqual(str.substr(1, 0), str.substr(0, 1)));
  EXPECT_FALSE(BoundsEqual(str.substr(1, 1), str.substr(1)));
  EXPECT_TRUE(BoundsEqual(str.substr(2, 0), str.substr(2, 0)));  // empty
  EXPECT_TRUE(BoundsEqual(str.substr(1, 1), str.substr(1, 1)));
  EXPECT_FALSE(BoundsEqual(str.substr(1, 2), str.substr(3, 2)));  // abutting
  EXPECT_FALSE(BoundsEqual(str.substr(3, 2), str.substr(1, 2)));  // abutting
  EXPECT_FALSE(BoundsEqual(str.substr(1, 2), str.substr(5, 2)));  // disjoint
  EXPECT_FALSE(BoundsEqual(str.substr(5, 2), str.substr(1, 2)));  // disjoint
  EXPECT_FALSE(BoundsEqual(str.substr(1, 4), str.substr(3, 4)));  // partial
  EXPECT_FALSE(BoundsEqual(str.substr(3, 4), str.substr(1, 4)));  // partial
}

}  // namespace
}  // namespace verible
