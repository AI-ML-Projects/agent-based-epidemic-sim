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

#ifndef AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURE_GENERATOR_H_
#define AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURE_GENERATOR_H_

#include <array>

#include "absl/time/time.h"
#include "agent_based_epidemic_sim/core/event.h"
#include "agent_based_epidemic_sim/core/exposure_generator.h"

namespace abesim {

class MicroExposureGenerator : public ExposureGenerator {
 public:
  virtual ~MicroExposureGenerator() = default;
  // Generate microexposures based on a uniform sampling.
  Exposure Generate(const absl::Time& start_time,
                    const absl::Duration& duration, float infectivity,
                    float symptom_factor) override;
};

}  // namespace abesim

#endif  // AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURE_GENERATOR_H_
