#pragma once
#include <Eigen/Core>
#include <vector>

namespace demo {

struct Scan {

  std::vector<Eigen::Vector3f> xyz;
  std::vector<float> intensity;
};

} // namespace demo