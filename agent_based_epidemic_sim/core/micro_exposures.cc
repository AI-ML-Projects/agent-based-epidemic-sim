#include "agent_based_epidemic_sim/core/micro_exposures.h"

#include "absl/time/time.h"
#include "agent_based_epidemic_sim/core/event.h"

namespace abesim {

const std::array<uint8, kNumberMicroExposureBuckets> GenerateMicroExposures(
    absl::Duration overlap) {
  std::array<uint8, kNumberMicroExposureBuckets> micro_exposure_counts = {};

  // TODO: Use a distribution of duration@distance once it is
  // figured out.
  // Generate counts for each bucket and never over assign
  // duration.
  const uint8 total_counts_to_assign = absl::ToInt64Minutes(overlap);

  if (total_counts_to_assign == 0) return micro_exposure_counts;

  const uint8 buckets_to_fill =
      std::min(kNumberMicroExposureBuckets, total_counts_to_assign);
  const uint8 counts_per_bucket = total_counts_to_assign / buckets_to_fill;

  for (auto i = 0; i < buckets_to_fill; i++) {
    micro_exposure_counts[i] = counts_per_bucket;
  }

  return micro_exposure_counts;
}

}  // namespace abesim
