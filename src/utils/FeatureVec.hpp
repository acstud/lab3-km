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