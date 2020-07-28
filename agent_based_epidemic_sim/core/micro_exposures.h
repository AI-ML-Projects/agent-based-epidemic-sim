#ifndef AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURES_H_
#define AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURES_H_

#include <array>

#include "absl/time/time.h"
#include "agent_based_epidemic_sim/core/event.h"

namespace abesim {

const std::array<uint8, kNumberMicroExposureBuckets> GenerateMicroExposures(
    absl::Duration overlap);

}  // namespace abesim

#endif  // AGENT_BASED_EPIDEMIC_SIM_CORE_MICRO_EXPOSURES_H_
