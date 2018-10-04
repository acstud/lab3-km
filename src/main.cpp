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

#include <iostream>
#include <fstream>
#include <memory>
#include <getopt.h>

#include "utils/Timer.hpp"
#include "utils/DataSet.hpp"
#include "krazy/KrazyMeans.hpp"

///@brief Program options
struct ProgramOptions {
  std::string input_file;
  std::string output_file;

  bool generate_example = false;
  bool generate_benchmark = false;
  bool plot_outputs = false;

  unsigned int clusters = 4;
  float scaling_factor = 1e-5;
  unsigned int threshold_iters = 64;

  unsigned long features = 2;
  unsigned long vectors = 1024;

  /// @brief Print usage information
  static void usage(char *argv[]) {
    std::cerr << "Usage: " << argv[0] << " -h -i <input> -o <output> -k K -t T -s S -pbe -f F -v V\n"
              << "\n"
              << "Example using all commands:\n"
              << argv[0] << "-e -b -p -k 10 -t 8 -s 0.1 -f 2 -v 1024 -i example.kmd -o labels.kml\n"
              << "Example for benchmark:\n"
              << argv[0] << "-b -k 32 -s 1e-3 -t 25 -i benchmark.kmd -o labels.kml\n"
              << "Options:\n"
                 "  -h            Show help and exit.\n"
                 "  -i <input>    Read data set from input file <input>.\n"
                 "  -o <output>   Write labels to output file <output>.\n"
                 "\n"
                 "KrazyMeans algorithm:\n"
                 "  -k K          Number of centroids.\n"
                 "  -t T          Threshold iterations.\n"
                 "  -s S          Distance scaling factor after threshold.\n"
                 "\n"
                 "Benchmarking and testing:\n"
                 "  -p            Save result in CSV files for Python plotting.\n"
                 "  -b            Generate the benchmark data set (benchmark.kmd).\n"
                 "\n"
                 "  -e            Generate an example data set for debugging purposes (example.kmd).\n"
                 "  -f F          Number of features for example data set.\n"
                 "  -v V          Number of vectors for example data set.\n";

    std::cerr.flush();
    exit(0);
  }

  ///@brief Generate a DataSet useful for Benchmarking
  void generateBenchmark() {
    auto ds = DataSet::random(42, 1024 * 512);
    ds->toFile("benchmark.kmd");
  }

  ///@brief Generate a DataSet useful for debugging/testing
  void generateExample() {
    auto ds = DataSet::random(features, vectors, clusters);
    ds->toFile("example.kmd");
  }

  ///@brief Run whatever was specified.
  void run() {
    if (generate_benchmark) generateBenchmark();
    if (generate_example) generateExample();

    if (!input_file.empty()) {
      Timer t;

      // Load data
      t.start();
      auto ds = DataSet::fromFile(input_file);
      t.stop();
      std::cout << "Loading dataset           : " << t.seconds() << " s." << std::endl;

      // Create KM context
      auto km = KrazyMeans(ds, clusters, threshold_iters, scaling_factor);

      // Initialize algorithm
      t.start();
      km.initialize();
      t.stop();
      std::cout << "Algorithm initialization  : " << t.seconds() << " s." << std::endl;

      // Run algorithm
      t.start();
      km.run(false);
      t.stop();
      std::cout << "Reached convergence after : " << t.seconds() << " s." << std::endl;
      std::cout << "Iterations                : " << km.iteration << std::endl;

      // Write labels to file
      if (!output_file.empty()) {
        t.start();
        km.dumpLabels(output_file);
        t.stop();
        std::cout << "Writing result            : " << t.seconds() << " s." << std::endl;
      } else {
        std::cerr << "No output file was specified." << std::endl;
      }

      if (plot_outputs) {
        std::ofstream points_out("points.csv");
        std::ofstream result_out("centroids.csv");

        if (points_out.good() && result_out.good()) {
          km.printState(points_out, result_out);
        } else {
          std::cerr << "Could not create CSV output files." << std::endl;
        }
      }
    } else {
      std::cerr << "No input file was specified." << std::endl;
    }
  }
};

int main(int argc, char *argv[]) {

  ProgramOptions po;

  if (argc == 1) {
    ProgramOptions::usage(argv);
  }

  // Use GNU getopt to parse command line options
  int opt;
  while ((opt = getopt(argc, argv, "hi:o:k:t:s:pebf:v:")) != -1) {
    switch (opt) {

      case 'h': {
        ProgramOptions::usage(argv);
        break;
      }

      case 'i': {
        po.input_file = std::string(optarg);
        break;
      }

      case 'o': {
        po.output_file = std::string(optarg);
        break;
      }

      case 'k': {
        char *end;
        po.clusters = (unsigned int) std::strtol(optarg, &end, 10);
        break;
      }

      case 't': {
        char *end;
        po.threshold_iters = (unsigned int) std::strtol(optarg, &end, 10);
        break;
      }

      case 'f': {
        char *end;
        po.features = (unsigned long) std::strtol(optarg, &end, 10);
        break;
      }

      case 'v': {
        char *end;
        po.vectors = (unsigned long) std::strtol(optarg, &end, 10);
        break;
      }

      case 's': {
        char *end;
        po.scaling_factor = std::strtof(optarg, &end);
        break;
      }

      case 'p': {
        po.plot_outputs = true;
        break;
      }

      case 'e': {
        po.generate_example = true;
        break;
      }

      case 'b': {
        po.generate_benchmark = true;
        break;
      }

      case '?':
        if ((optopt == 'i') || (optopt == 'o')) {
          std::cerr << "Options -i and -o require an argument." << std::endl;
          ProgramOptions::usage(argv);
        }
        break;

      default: {
        std::cerr << "Some invalid option was supplied." << std::endl;
        ProgramOptions::usage(argv);
        break;
      }

    }
  }

  po.run();

  return 0;
}
