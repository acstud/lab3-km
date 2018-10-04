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

#include <fstream>
#include <iostream>

#include "DataSet.hpp"
#include "RandomGenerator.hpp"

void DataSet::addVector(FeatureVec &f) {
  if (num_features == f.size()) {
    vectors.push_back(f);
  } else {
    throw std::runtime_error("Feature vector is of different length than number of required features in DataSet.");
  }
}

void DataSet::addVector(std::vector<float> &f) {
  if (num_features == f.size()) {
    vectors.emplace_back(f);
  } else {
    throw std::runtime_error("Feature vector is of different length than number of required features in DataSet.");
  }
}

void DataSet::toFile(std::string file_name) {
  // Open the file
  std::ofstream file(file_name, std::ios::binary);

  if (file.good()) {
    // Write number of features
    file.write((char *) &num_features, sizeof(size_t));
    // Write size
    auto s = size();
    file.write((char *) &s, sizeof(size_t));

    // Write vectors
    for (size_t v = 0; v < size(); v++) {
      for (size_t f = 0; f < num_features; f++) {
        file.write((char *) &vector(v)[f], sizeof(float));
      }
    }
  } else {
    throw std::runtime_error("Could not write to file.");
  }
}

std::shared_ptr<DataSet> DataSet::fromFile(const std::string &file_name) {
  auto ds = std::make_shared<DataSet>();

  std::ifstream file(file_name, std::ios::binary);

  if (file.good()) {
    // Read num features
    file.read((char *) &ds->num_features, sizeof(size_t));
    size_t size;
    // Read number of vectors
    file.read((char *) &size, sizeof(size_t));
    ds->vectors.reserve(size);
    for (size_t v = 0; v < size; v++) {
      std::vector<float> vec(ds->num_features);
      for (size_t f = 0; f < ds->num_features; f++) {
        file.read((char *) &vec[f], sizeof(float));
      }
      ds->addVector(vec);
    }
    return ds;
  } else {
    throw std::runtime_error("Could not load from file");
  }

}
std::shared_ptr<DataSet> DataSet::random(size_t num_features, size_t num_vectors, int num_clusters) {

  // Fully randomize feature vectors if number of centroids is negative.
  if (num_clusters == -1) {

    UniformRandomGenerator<float> rg(1);

    auto ds = std::make_shared<DataSet>(num_features);

    for (int i = 0; i < num_vectors; i++) {
      std::vector<float> vector(ds->num_features);
      for (int f = 0; f < ds->num_features; f++) {
        vector[f] = rg.next();
      }
      ds->addVector(vector);
    }
    return ds;
  } else {
    NormalRandomGenerator<float> rg(1);
    // Sort of randomize the data to lie around some centroids
    std::vector<std::vector<float>> aimed_centroids(
        static_cast<unsigned long>(num_clusters),
        std::vector<float>(num_features));

    // Generate some random centroids
    for (auto &c : aimed_centroids) {
      for (auto &f: c) {
        f = rg.next();
      }
    }

    // Create a new data set
    auto ds = std::make_shared<DataSet>(num_features);

    for (int i = 0; i < num_vectors; i++) {
      std::vector<float> vector(ds->num_features);
      for (int f = 0; f < ds->num_features; f++) {
        vector[f] = aimed_centroids[i % num_clusters][f] + 0.25f * rg.next();
      }
      ds->addVector(vector);
    }
    return ds;
  }
}
