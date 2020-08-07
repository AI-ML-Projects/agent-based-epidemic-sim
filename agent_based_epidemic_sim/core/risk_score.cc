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

#include "agent_based_epidemic_sim/core/risk_score.h"

#include "absl/memory/memory.h"
#include "absl/time/time.h"

namespace abesim {

namespace {

class NullRiskScore : public RiskScore {
 public:
  void AddHealthStateTransistion(HealthTransition transition) override {}
  void AddExposures(absl::Span<const Exposure* const> exposures) override {}
  void AddExposureNotification(const Contact& contact,
                               const TestResult& result) override {}
  void AddTestResult(const TestResult& result) override {}

  VisitAdjustment GetVisitAdjustment(const Timestep& timestep,
                                     int64 location_uuid) const override {
    return {.frequency_adjustment = 1.0, .duration_adjustment = 1.0};
  }
  TestPolicy GetTestPolicy(const Timestep& timestep) const override {
    return {.should_test = false,
            .time_requested = absl::InfiniteFuture(),
            .latency = absl::InfiniteDuration()};
  }

  ContactTracingPolicy GetContactTracingPolicy() const override {
    return {.report_recursively = false, .send_positive_test = false};
  }

  absl::Duration ContactRetentionDuration() const override {
    return absl::ZeroDuration();
  }
};

}  // namespace

std::unique_ptr<RiskScore> NewNullRiskScore() {
  return absl::make_unique<NullRiskScore>();
}

}  // namespace abesim
