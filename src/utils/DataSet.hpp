#pragma once

#include <vector>
#include <memory>
#include <stdexcept>

#include "FeatureVec.hpp"

// A data set with vectors
struct DataSet {
  size_t num_features = 0;
  std::vector<FeatureVec> vectors;

  ///@brief Construct a new data set with \p num_features features in the feature vectors.
  explicit DataSet(size_t num_features = 1) : num_features(num_features) {};

  /**
   * @brief     Add a feature vector to the data set.
   * @param f   The feature vector (a copy is made).
   */
  void addVector(FeatureVec &f);

  /**
   * @brief     Add a feature vector to the data set.
   * @param f   The feature vector (a copy is made).
   */
  void addVector(std::vector<float> &f);

  ///@brief Access the vector at index \p idx
  inline FeatureVec &vector(size_t idx) { return vectors[idx]; }

  ///@brief Access the vector at index \p idx
  inline FeatureVec &operator[](size_t idx) { return vector(idx); }

  ///@brief Return the number of feature vectors in the data set.
  inline size_t size() const { return vectors.size(); }

  ///@brief Write the DataSet to file
  void toFile(std::string file_name);

  ///@brief Load a DataSet from a file
  static std::shared_ptr<DataSet> fromFile(const std::string &file_name);

  ///@brief Create a random DataSet
  static std::shared_ptr<DataSet> random(size_t features, size_t vectors, int num_clusters=-1);
};