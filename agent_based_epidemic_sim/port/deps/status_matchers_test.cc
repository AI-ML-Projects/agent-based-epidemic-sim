// Copyright 2020 Google LLC
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

#include "agent_based_epidemic_sim/port/deps/status_matchers.h"

#include "absl/status/status.h"
#include "agent_based_epidemic_sim/port/deps/status_macros.h"
#include "agent_based_epidemic_sim/port/deps/statusor.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {

using abesim::IsOkAndHolds;
using abesim::StatusIs;
using absl::OkStatus;
using testing::_;
using testing::Not;

absl::Status AbortedStatus() {
  return absl::Status(absl::StatusCode::kAborted, "aborted");
}

abesim::StatusOr<int> OkStatusOr(int n) { return n; }

abesim::StatusOr<int> AbortedStatusOr() { return AbortedStatus(); }

TEST(StatusMatcher, Macros) {
  PANDEMIC_EXPECT_OK(OkStatus());
  PANDEMIC_ASSERT_OK(OkStatus());
}

TEST(StatusMatcher, IsOkAndHolds) {
  EXPECT_THAT(OkStatusOr(15), IsOkAndHolds(15));
  EXPECT_THAT(OkStatusOr(15), Not(IsOkAndHolds(0)));
  EXPECT_THAT(AbortedStatusOr(), Not(IsOkAndHolds(0)));

  // Weird usage, but should work
  EXPECT_THAT(OkStatusOr(15), IsOkAndHolds(_));
}

TEST(StatusMatcher, StatusIs) {
  EXPECT_THAT(AbortedStatus(), StatusIs(absl::StatusCode::kAborted, "aborted"));
  EXPECT_THAT(AbortedStatus(), StatusIs(absl::StatusCode::kAborted, _));
  EXPECT_THAT(AbortedStatus(), StatusIs(_, "aborted"));
  EXPECT_THAT(AbortedStatus(), StatusIs(_, _));

  EXPECT_THAT(AbortedStatus(), StatusIs(absl::StatusCode::kAborted));
  EXPECT_THAT(AbortedStatus(), StatusIs(Not(absl::StatusCode::kOk)));
  EXPECT_THAT(AbortedStatus(), StatusIs(_));

  EXPECT_THAT(AbortedStatusOr(),
              StatusIs(absl::StatusCode::kAborted, "aborted"));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(absl::StatusCode::kAborted, _));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_, "aborted"));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_, _));

  EXPECT_THAT(AbortedStatusOr(), StatusIs(absl::StatusCode::kAborted));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(Not(absl::StatusCode::kOk)));
  EXPECT_THAT(AbortedStatusOr(), StatusIs(_));

  // Weird usages, but should work.
  EXPECT_THAT(OkStatusOr(15), StatusIs(absl::StatusCode::kOk, ""));
  EXPECT_THAT(OkStatusOr(15), StatusIs(absl::StatusCode::kOk, _));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_, ""));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_, _));

  EXPECT_THAT(OkStatusOr(15), StatusIs(absl::StatusCode::kOk));
  EXPECT_THAT(OkStatusOr(15), StatusIs(Not(absl::StatusCode::kAborted), _));
  EXPECT_THAT(OkStatusOr(15), StatusIs(_));

  EXPECT_THAT(OkStatus(), StatusIs(absl::StatusCode::kOk, ""));
  EXPECT_THAT(OkStatus(), StatusIs(absl::StatusCode::kOk, _));
  EXPECT_THAT(OkStatus(), StatusIs(_, ""));
  EXPECT_THAT(OkStatus(), StatusIs(_, _));

  EXPECT_THAT(OkStatus(), StatusIs(absl::StatusCode::kOk));
  EXPECT_THAT(OkStatus(), StatusIs(Not(absl::StatusCode::kAborted), _));
  EXPECT_THAT(OkStatus(), StatusIs(_));
}

}  // namespace
