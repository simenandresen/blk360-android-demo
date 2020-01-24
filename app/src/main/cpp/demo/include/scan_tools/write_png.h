#pragma once
#include <string>

namespace demo {

bool writePng(const std::string &path, const std::vector<float> &data, size_t image_size);
}
