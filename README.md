# Advanced Computing Systems 2018-2019 Lab 3 - Accelerating a clustering algorithm

This repository contains all files and explanations needed to perform
the third lab of the EE4C07 - Advanced Computing Systems course at the
Delft University of Technology.

## What do I have to do for Lab 3?

* Download the baseline project.
* Improve the throughput of the project using any of the techniques learned in 
  the course, as much as you can. 
* Write a report.
* Turn in your report.
* Rejoice with the knowledge that you have gained.

## What does the baseline project do?

The baseline project implements a variant of the K-Means clustering algorithm.
The K-means clustering algorithm takes a data set consisting of feature vectors.
It then attempts to cluster feature vectors that are nearby each other into K 
clusters, where K is parameterizable.

For example, suppose we take a dataset of a group of students, where the feature
vectors contain: {age, height, grade}. We then want to create two groups
of students with the most overlapping features. The idea about K-means clustering
is that we will create groups of the feature vectors that amongst each other have
the smallest Euclidean distance.

There is ample literature available online about K-means clustering. The baseline
example implements the pseudocode found in 
[this wikibook](https://en.wikibooks.org/wiki/Data_Mining_Algorithms_In_R/Clustering/K-Means). 
However, there is one difference to make this variant of K-means clustering
a bit more unique.

Specifically, instead of supplying a maximum number of iterations, you can
provide a threshold number of iterations. After the threshold, all Euclidean
distances will be scaled for vectors wanting to switch to another cluster.
This causes the clustering to converge more quickly, although the accuracy will drop.

Whether or not this change is totally insane or actually has some merit, 
is still undecided. Therefore, the algorithm variant is called KrazyMeans, 
just to be sure.

## What should I do with the baseline project?

You should read the baseline source code and figure out how to program works.

Then, you must:
 
* Improve the throughput of the project using **any of the techniques learned in
  the course** as much as possible.
  * You MUST use OpenCL or CUDA for the major run-time components.
  * You can use SIMD, OpenMP for some other parts.
  * You cannot use any external libraries unless explicit permission was
  given.
  
* You can **not** change the way the algorithm works in a functional sense. E.g. you
cannot change it to another variant of K-means. The distance scaling functionality
MUST be included in your implementation. If you are unsure if some algorithmic
change is allowed, ask the lab instructors - this is _your_ responsibility. 

## What data set should I use for testing/debugging?

The code contains some functions to generate an example data set which can be 
visualized through a [Python script](plot/plot_cluster.py). Using the `-p` option
you can output the clustering context as some CSV files. The scripts will draw
an image of those. Don't use this on large data sets. It only works for feature
vectors with two dimensions.

You can run it with:

```console
python3 plot_cluster.py
```

The script expects the `points.csv` and `centroids.csv` files in its working
directory. It also requires some packages like Pandas and Matplotlib.

## What data set should I use for benchmarking / report discussions?

Eventually you should benchmark your implementation using a generated data set using
the `-b` option. As the feature vectors will be uniformly distributed throughout the
feature space, any clustering of this data set will on average result in that every
cluster contains about as many points. 

You can use this to test your own implementation. There is no functional test supplied
with this code, you need to write a test yourself to verify your results.

The figure of merit will be the run-time when running the improved program with ...

```console
./krazymeans -k 24 -s 0e-4 -t 30 -i benchmark.kmd -o labels.kml
```

... where `benchmark.kmd` was generated using the `-b` option.

## Can I share some of my knowledge with people outside my group?

There are some bonus points awarded to the groups with the fastest implementation.

You can decide whether sharing your industry secrets on optimizations of this 
algorithm with other groups is a smart thing to do ;-)

* 1st place: +15% on lab 3 grade
* 2nd place: +10% on lab 3 grade
* 3rd place: +5% on lab 3 grade

## What files can I change?

You can change any files in the project.

## Can I fork this repository as a student?

Answer: __NO, DO NOT FORK IT (publicly)__. 
See the Lab 1 statement about this.

## Is there a template for the lab report?

You can use the templates from lab 1 and lab 2, and modify them according to what
you want to convey through your report. However, these things MUST be included in
the discussion of your report:

1. An application profile of the baseline / intermediate versions.
2. An estimation on what performance can be gained after improving specific parts, 
  based on the profiles.
3. A short discussion on how you've improved specific parts.
4. Go back to 1 and repeat until you've squeezed out as much throughput as possible.

Other than that we will use exactly the same rubric as supplied in lab 1 to grade the
lab work and report. So make sure you convey the information needed for the lab instructors
to base a grade on.
