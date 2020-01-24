#pragma once

#include <vector>
#include <Eigen/Core>

namespace demo {
    std::vector<Eigen::Vector3f>
    downsample(const std::vector<Eigen::Vector3f> &points, float resolution);
}
