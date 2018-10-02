#pragma once

#include <memory>
#include <iostream>

#include "../utils/DataSet.hpp"
#include "../utils/RandomGenerator.hpp"

/**
 * @brief KrazyMeans context used for Feature Vector clustering.
 *
 * KrazyMeans is a modified version of the K-means algorithm that does not work with a maximum number of iterations.
 * Instead, to reach faster convergence, a threshold number of iterations can be supplied. When the threshold iteration
 * is reached, the distances of points that want to swap to another cluster during the updating of the labels are
 * multiplied by some factor. This factor grows linearly with the number of iterations over the threshold. Hereby, the
 * likeliness of swapping a point to another cluster is decreased, allowing the clustering algorithm to converge more
 * quickly.
 *
 * This algorithm is called KrazyMeans, because it is almost identical to K-Means, but contains the aforementioned
 * rather crazy modification, to make the assignment relatively unique and not easily copied from some other source
 * for the Advanced Computing Systems course at TU Delft.
 */
struct KrazyMeans {

  ///@brief The data set to work on.
  const std::shared_ptr<DataSet> data_set;

  ///@brief The number of clusters to generate a clustering for.
  unsigned int num_clusters = 0;

  ///@brief The iteration threshold before starting distance scaling.
  unsigned int scale_threshold_iterations = 0;

  ///@brief The factor at which to scale the distance per iteration.
  float scale_factor = 0.01;

  ///@brief The labels of the feature vectors.
  std::vector<size_t> labels;

  ///@brief The current centroids.
  std::vector<FeatureVec> centroids;

  ///@brief Whether the algorithm has converged.
  bool converged = false;

  ///@brief The iteration at which the algorithm is operating currently.
  unsigned int iteration = 0;

  /**
   * @brief Construct a new KrazyMeans context.
   *
   * @param data_set                The data set to work on.
   * @param num_clusters            The number of clusters to calculate centroids for.
   * @param scale_threshold_iters   Distance scaling when iterations threshold is reached.
   * @param scale_factor            Distance scaling factor after iterations threshold is reached.
   */
  KrazyMeans(const std::shared_ptr<DataSet> &data_set,
             unsigned int num_clusters,
             unsigned int scale_threshold_iters,
             float scale_factor);;

  /**
   * @brief Find the centroid closest to the feature vector /p vec.
   * @param vec A feature vector.
   * @return The index of the closest centroid.
   */
  size_t findClosestCentroidIndex(size_t vec_index);

  ///@brief Select the centroids to be random points in the data set.
  void selectRandomCentroids();

  /**
   * @brief Update the labels of the feature vectors in the data set.
   * @return Whether labels were changed. Useful to check for convergence.
   */
  bool updateLabels();

  ///@brief Reset the centroid feature values to zero.
  void clearCentroids();

  ///@brief Calculate the new position of the centroids according to the labels.
  void updateCentroids();

  ///@brief Initialize the KMeans clustering algorithm
  void initialize();

  ///@brief Run a single iteration.
  void iterate();

  /// @brief Run all iterations until convergence.
  /// Non-quiet mode doesn't have to be implemented by students.
  void run(bool quiet=true);

  ///@brief Print the state of the clustering algorithm. Not recommended for large data sets.
  void printState(std::ostream &labels_out = std::cout, std::ostream &centroids_out = std::cout);

  ///@brief Dump the labels to a file.
  void dumpLabels(std::string file_name);
};