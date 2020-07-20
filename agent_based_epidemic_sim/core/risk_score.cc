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

  // Gets the policy to be used when sending contact reports.
  ContactTracingPolicy GetContactTracingPolicy() const override {
    return {.report_recursively = false, .send_positive_test = false};
  }

  // Gets the duration for which to retain contacts.
  absl::Duration ContactRetentionDuration() const override {
    return absl::ZeroDuration();
  }
};

}  // namespace

std::unique_ptr<RiskScore> NewNullRiskScore() {
  return absl::make_unique<NullRiskScore>();
}

}  // namespace abesim
