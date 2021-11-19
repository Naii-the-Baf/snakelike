#include "common.h"

namespace Common{
std::random_device rand_dev{};
std::mt19937 mt_rand(rand_dev());
} //namespace Common
