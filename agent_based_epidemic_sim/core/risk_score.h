#ifndef AGENT_BASED_EPIDEMIC_SIM_CORE_RISK_SCORE_H_
#define AGENT_BASED_EPIDEMIC_SIM_CORE_RISK_SCORE_H_

#include <memory>

#include "agent_based_epidemic_sim/core/event.h"
#include "agent_based_epidemic_sim/core/timestep.h"

namespace abesim {

class RiskScore {
 public:
  // TODO: We should consider whether to convert this to AddHealthState.
  // Currently GetVisitAdjustment does not take the actual time of the visit
  // so it's not clear what to do other than use the latest health state
  // (which is what all current RiskScore implementations do).  On the other
  // hand if we make GetVisitAdjustment take a visit time, then risk score
  // implementations would likely have to save the history of transitions to
  // properly handle it, which is undesirable since it's duplicated by
  // Agent implementations.  It's also difficult because we first generate
  // visits, then in the agent, split the visits around health transition
  // boundaries.  We would have to decide what to do, one option would be for
  // visit generators to cut visits short at onset of symptoms.
  virtual void AddHealthStateTransistion(HealthTransition transition) = 0;
  virtual void AddExposures(absl::Span<const Exposure* const> exposures) = 0;
  virtual void AddExposureNotification(const Contact& contact,
                                       const TestResult& result) = 0;
  virtual void AddTestResult(const TestResult& result) = 0;

  struct VisitAdjustment {
    float frequency_adjustment;
    float duration_adjustment;

    friend bool operator==(const VisitAdjustment& a, const VisitAdjustment& b) {
      return (a.frequency_adjustment == b.frequency_adjustment &&
              a.duration_adjustment == b.duration_adjustment);
    }

    friend bool operator!=(const VisitAdjustment& a, const VisitAdjustment& b) {
      return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& strm,
                                    const VisitAdjustment& visit_adjustment) {
      return strm << "{" << visit_adjustment.frequency_adjustment << ", "
                  << visit_adjustment.duration_adjustment << "}";
    }
  };
  // Get the adjustment a particular agent should make to it's visits to the
  // given location.
  // Note that different agents can have different policies.  For exmample
  // an essential employee may see no adjustment, whereas a non-essential
  // employee may be banned from the same location.
  virtual VisitAdjustment GetVisitAdjustment(const Timestep& timestep,
                                             int64 location_uuid) const = 0;

  // Encapsulates whether and how to request a test. Contains the following:
  // - whether a test should be conducted
  // - the time at which the test is requested
  // - the duration for receiving a result from the test.
  struct TestPolicy {
    bool should_test;
    absl::Time time_requested;
    absl::Duration latency;

    friend bool operator==(const TestPolicy& a, const TestPolicy& b) {
      return (a.should_test == b.should_test &&
              a.time_requested == b.time_requested && a.latency == b.latency);
    }

    friend bool operator!=(const TestPolicy& a, const TestPolicy& b) {
      return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& strm,
                                    const TestPolicy& test_policy) {
      return strm << "{" << test_policy.should_test << ","
                  << test_policy.time_requested << "," << test_policy.latency
                  << "}";
    }
  };
  virtual TestPolicy GetTestPolicy(const Timestep& timestep) const = 0;

  // Encapsulates which contact reports to forward.
  struct ContactTracingPolicy {
    bool report_recursively;
    bool send_positive_test;

    friend bool operator==(const ContactTracingPolicy& a,
                           const ContactTracingPolicy& b) {
      return (a.report_recursively == b.report_recursively &&
              a.send_positive_test == b.send_positive_test);
    }

    friend bool operator!=(const ContactTracingPolicy& a,
                           const ContactTracingPolicy& b) {
      return !(a == b);
    }

    friend std::ostream& operator<<(
        std::ostream& strm,
        const ContactTracingPolicy& contact_tracing_policy) {
      return strm << "{" << contact_tracing_policy.report_recursively << ", "
                  << contact_tracing_policy.send_positive_test << "}";
    }
  };
  // Gets the policy to be used when sending contact reports.
  // TODO: Should take a timestep.
  virtual ContactTracingPolicy GetContactTracingPolicy() const = 0;

  // Gets the duration for which to retain contacts.
  virtual absl::Duration ContactRetentionDuration() const = 0;

  virtual ~RiskScore() = default;
};

// Samples RiskScore instances.
class RiskScoreGenerator {
 public:
  // Get a policy for the next worker.
  virtual std::unique_ptr<RiskScore> NextRiskScore() = 0;
  virtual ~RiskScoreGenerator() = default;
};

std::unique_ptr<RiskScore> NewNullRiskScore();

}  // namespace abesim

#endif  // AGENT_BASED_EPIDEMIC_SIM_CORE_RISK_SCORE_H_
