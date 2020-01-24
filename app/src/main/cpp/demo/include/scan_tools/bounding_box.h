#include <Eigen/Core>
#include <vector>

namespace demo {

    void boundingBox(const std::vector <Eigen::Vector3f> &points,
                     Eigen::Vector3f &min_point, Eigen::Vector3f &max_point);
}