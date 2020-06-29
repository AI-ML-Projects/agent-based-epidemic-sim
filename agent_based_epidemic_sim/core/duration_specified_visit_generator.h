/*
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef THIRD_PARTY_AGENT_BASED_EPIDEMIC_SIM_CORE_DURATION_SPECIFIED_VISIT_GENERATOR_H_
#define THIRD_PARTY_AGENT_BASED_EPIDEMIC_SIM_CORE_DURATION_SPECIFIED_VISIT_GENERATOR_H_

#include "absl/random/random.h"
#include "absl/time/time.h"
#include "agent_based_epidemic_sim/core/event.h"
#include "agent_based_epidemic_sim/core/integral_types.h"
#include "agent_based_epidemic_sim/core/timestep.h"
#include "agent_based_epidemic_sim/core/visit.h"
#include "agent_based_epidemic_sim/core/visit_generator.h"

namespace pandemic {

// Generates visits to the given set of locations with durations using the
// given sampler.
// All locations are covered in a round-robin in each call to GenerateVisit,
// with the total duration normalized to sum to timestep.
// Locations can be repeated.
struct LocationDuration {
  int64 location_uuid;
  // The adjustment parameter is a float from [0-1] and should
  // linearly scale the mean of the sample.
  std::function<float(float adjustment)> sample_duration;
};

class DurationSpecifiedVisitGenerator : public VisitGenerator {
 public:
  explicit DurationSpecifiedVisitGenerator(
      const std::vector<LocationDuration>& location_durations)
      : location_durations_(location_durations) {}

  void GenerateVisits(const Timestep& timestep, const PublicPolicy* policy,
                      HealthState::State current_health_state,
                      const ContactSummary& contact_summary,
                      std::vector<Visit>* visits) override;

 private:
  std::vector<LocationDuration> location_durations_;
  absl::BitGen gen_;
};

}  // namespace pandemic

#endif  // THIRD_PARTY_AGENT_BASED_EPIDEMIC_SIM_CORE_DURATION_SPECIFIED_VISIT_GENERATOR_H_
