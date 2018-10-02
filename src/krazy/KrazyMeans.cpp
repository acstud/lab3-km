#include <fstream>
#include "KrazyMeans.hpp"
#include "../utils/Timer.hpp"

KrazyMeans::KrazyMeans(const std::shared_ptr<DataSet> &data_set,
                       unsigned int num_clusters,
                       unsigned int scale_threshold_iters,
                       float scale_factor)
    : data_set(data_set),
      num_clusters(num_clusters),
      scale_factor(scale_factor),
      scale_threshold_iterations(scale_threshold_iters) {
  // Initialize all labels to 0
  labels.reserve(data_set->vectors.size());
  for (size_t i = 0; i < data_set->vectors.size(); i++) {
    labels.push_back(0);
  }

  // Initialize the centroids to be all zero.
  centroids = std::vector<FeatureVec>(num_clusters, FeatureVec(data_set->num_features, 0.0f));
}

size_t KrazyMeans::findClosestCentroidIndex(size_t vec_index) {
  float closest = INFINITY;
  size_t index = 0;

  float factor = 1.0f;

  // When we reach the iterations threshold, penalize the distance for any vector switching to another centroid
  // This will cause faster convergence
  if (iteration >= scale_threshold_iterations) {
    factor = 1.0f + (float) (iteration - scale_threshold_iterations) * scale_factor;
  }

  for (size_t c = 0; c < centroids.size(); c++) {
    float dist = calculateEuclideanDistance(data_set->vector(vec_index), centroids[c]);

    // Penalize potential swapping of labels with a factor
    if (labels[vec_index] != c) {
      dist = dist * factor;
    }

    if (dist < closest) {
      closest = dist;
      index = c;
    }
  }
  return index;
}

void KrazyMeans::selectRandomCentroids() {
  UniformRandomGenerator<long> rg;
  // For each cluster centroid, randomly select a feature vector as initialization.
  for (auto &vector : centroids) {
    vector = data_set->vectors[rg.next() % data_set->size()];
  }
}

bool KrazyMeans::updateLabels() {
  auto updated = false;
  // For each feature vector, find the current closest centroid
  for (size_t i = 0; i < data_set->size(); i++) {
    auto closest = findClosestCentroidIndex(i);
    if (labels[i] != closest) {
      labels[i] = closest;
      updated = true;
    }
  }
  return updated;
}

void KrazyMeans::clearCentroids() {
  for (auto &centroid : centroids) {
    centroid.clear();
  }
}

void KrazyMeans::updateCentroids() {
  // Extremely naive implementation to update centroids.

  // Clear the centroids
  clearCentroids();

  // Iterate over all centroids
  for (size_t c = 0; c < centroids.size(); c++) {
    // Keep track of amount of feature vectors assigned to this centroid.
    size_t num_assigned = 0;

    // Find all vectors that belong to this centroid label
    for (size_t v = 0; v < data_set->size(); v++) {
      // If this vector is assigned to this centroid...
      if (labels[v] == c) {
        num_assigned++;
        // Accumulate the centroid feature values
        for (size_t f = 0; f < centroids[c].size(); f++) {
          centroids[c][f] += data_set->vectors[v][f];
        }
      }
    }
    // Average out each feature if new assignments were made
    if (num_assigned != 0) {
      for (size_t f = 0; f < data_set->num_features; f++) {
        centroids[c][f] /= (float) num_assigned;
      }
    }
  }
}

void KrazyMeans::initialize() {
  selectRandomCentroids();
  updateLabels();
}

void KrazyMeans::iterate() {
  updateCentroids();
  converged = !updateLabels();
  iteration++;
}

void KrazyMeans::run(bool quiet) {
  while (!converged) {
    // Only quiet mode should be optimized. Non-quiet mode is not required.
    if (quiet) {
      // Quiet mode
      iterate();
    }
    else {
      // Non quiet mode
      Timer t;
      t.start();
      iterate();
      t.stop();
      std::cout << iteration << " - " << t.seconds() << " s." << std::endl;
    }
  }
}

void KrazyMeans::printState(std::ostream &labels_out, std::ostream &centroids_out) {
  // Print labels for all vectors
  for (size_t v = 0; v < data_set->size(); v++) {
    labels_out << v << ", " << labels[v] << ", " << data_set->vector(v).toString() << std::endl;
  }

  // Print centroids
  for (size_t c = 0; c < num_clusters; c++) {
    centroids_out << c << ", " << centroids[c].toString() << std::endl;
  }
}

void KrazyMeans::dumpLabels(std::string file_name) {
  std::ofstream file(file_name, std::ios::binary);

  // Write labels as uint16_t
  for (size_t v = 0; v < data_set->size(); v++) {
    file.write((char *) &labels[v], sizeof(size_t));
  }
}
