#pragma once
#include <vector>
#include <openpose/headers.hpp>
#define OPENPOSE_FLAGS_DISABLE_PRODUCER
#define OPENPOSE_FLAGS_DISABLE_DISPLAY
#include <memory>

std::shared_ptr<op::Datum> estimateKeypoints(const std::vector<std::vector<float>> &frame);
