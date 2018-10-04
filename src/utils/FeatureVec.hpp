// Copyright 2018 Delft University of Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <cmath>

/**
* @brief A feature vector
*
* This structure just wraps around a std::vector<float>, but provides some additional convenience functions.
*/
struct FeatureVec {
  ///@brief The values of this feature vector
  std::vector<float> values;
  FeatureVec() = default;
  explicit FeatureVec(std::vector<float> vec) : values(std::move(vec)) {}
  FeatureVec(size_t num_features, float initial_value);
  FeatureVec(std::initializer_list<float> l);

  ///@brief Access the element at index \p idx
  inline float &operator[](size_t idx) { return values[idx]; }

  ///@brief Return the size of this feature vector
  inline size_t size() const { return values.size(); }

  ///@brief Clear this feature vector.
  void clear();

  ///@brief Print this vector to stdout
  std::string toString();
};

/**
 * Calculate the Euclidean distance between feature vector A and B
 * @param a A vector
 * @param b Another vector
 * @return The Euclidean Distance
 */
float calculateEuclideanDistance(FeatureVec a, FeatureVec b);