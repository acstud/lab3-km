import matplotlib.pyplot as plt
import numpy as np
import pandas
import math
import csv

f_points = open("points.csv")
f_centroids = open("centroids.csv")

points = pandas.read_csv(f_points, header=None)
centroids = pandas.read_csv(f_centroids, header=None)

# Obtain dimensions
num_vectors = points.shape[0]
num_features = points.shape[1] - 2
num_centroids = centroids.shape[0]
num_centroid_features = centroids.shape[1] - 1

# Generate all colors of the rainbow
colors = np.zeros([num_centroids,3])
colors[0:num_centroids,0] = 0.6+0.4*np.sin(2*np.pi*np.arange(num_centroids)/num_centroids+0/3*2*np.pi)
colors[0:num_centroids,1] = 0.6+0.4*np.sin(2*np.pi*np.arange(num_centroids)/num_centroids+1/3*2*np.pi)
colors[0:num_centroids,2] = 0.6+0.4*np.sin(2*np.pi*np.arange(num_centroids)/num_centroids+2/3*2*np.pi)

if (num_features != 2) or (num_centroid_features != 2):
  exit("Plotting only works for 2D feature vectors.")

fig, ax = plt.subplots()

# Plot vectors
for v in range(0, num_vectors):
  label = int(points.values[v,1])
  ax.plot(points.values[v, 2], points.values[v, 3], marker='x', color=0.8*colors[label,0:3])

# Plot centroids and a little label
for c in range(0, num_centroids):
  ax.plot(centroids.values[c, 1], centroids.values[c, 2], marker='o', color=colors[c,0:3], markersize=12)
  ax.text(centroids.values[c, 1]-0.05, centroids.values[c, 2]-0.08, str(c))

plt.show()
