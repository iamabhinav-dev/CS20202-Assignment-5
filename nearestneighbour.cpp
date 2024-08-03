/**
* @file DataVector.h
* @brief Define the functionality of KNearestNeighbour
* 
*The provided C++ program performs a nearest neighbor search on a dataset using the k-nearest
*neighbors (KNN) algorithm. The program reads two datasets, namely "train.csv" and "test.csv,"
*representing training and test data, respectively. The training dataset is used to build a model,
*and the test dataset is utilized to find the k-nearest neighbors for each data point. The user 
*can specify different values of k (number of neighbors to consider) for the search. The program 
*converts the dataset into vectors of DataVector objects, where each DataVector encapsulates a set
*of numeric values. The nearest neighbor search is conducted using a custom VectorDataset class, 
*and the results are displayed, showing the nearest neighbors for each data point in the test dataset. 
*The program reports the sizes of the training and test vectors, ensures that the vectors are non-empty, 
*and validates the chosen k value. Additionally, the execution time for each k value is measured using 
*chrono library, and the total time taken for the nearest neighbor search is displayed. This program
*serves as a versatile tool for exploring different k values and assessing the impact on the KNN
*algorithm's performance on the given datasets.
* 
* @author Abhinav Kumar Singh
* 
 * System Information 
 *OS: Ubuntu 22.04.3 LTS x86_64 
 *Host: ASUS TUF Gaming A15 FA506ICB_FA506ICB 1.0 
 *Kernel: 6.5.0-15-generic
 *Packages: 1779 (dpkg), 11 (snap) 
 *Shell: bash 5.1.16 
 *Resolution: 1920x1080 
 *DE: GNOME 42.9 
 *WM: Mutter 
 *WM Theme: Adwaita 
 *Theme: Yaru-blue-dark [GTK2/3] 
 *Icons: Yaru-blue [GTK2/3] 
 *Terminal: gnome-terminal 
 *CPU: AMD Ryzen 7 4800H with Radeon Graphics (16) @ 4.300G 
 *GPU: NVIDIA GeForce RTX 3050 Mobile 
 *GPU: AMD ATI 05:00.0 Renoir 
 *Memory: 2452MiB / 7343MiB
 * 
 * Output Time For Some k's:
 * K=2 Time Taken:164ms
 * K=5 Time Taken:291ms
 * K=7 Time Taken:391ms
 * K=10 Time Taken:442ms    
*/

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "VectorDataset.h"
using namespace std;

// Function declarations
VectorDataset readDatasetFromFile(const string &filename);
VectorDataset knearestneighbor(const vector<DataVector> &trainVectors,
                               const vector<DataVector> &testVectors, int k);

int main() {
  //Start Timer
   auto start = chrono::high_resolution_clock::now();
  // Read training dataset from datasettrain.csv
  VectorDataset trainDataset = readDatasetFromFile("train.csv");

  // Read test dataset from datasettest.csv
  VectorDataset testDataset = readDatasetFromFile("test.csv");

  // Convert VectorDataset to vectors of DataVector
  vector<DataVector> trainVectors;
  for (int i = 0; i < trainDataset.size(); ++i) {
    trainVectors.push_back(trainDataset.getDataVector(i));
  }

  vector<DataVector> testVectors;
  for (int i = 0; i < testDataset.size(); ++i) {
    testVectors.push_back(testDataset.getDataVector(i));
  }

  // Specify k for nearest neighbor search
  int k = 10;

  // Print sizes of train and test vectors
  cout << "Train Vector Size: " << trainVectors.size() << endl;
  cout << "Test Vector Size: " << testVectors.size() << endl;

  // Check if the vectors are non-empty
  if (trainVectors.empty() || testVectors.empty()) {
    cerr << "Error: Train or test vectors are empty." << endl;
    return 1;
  }

  
  // Check if k is within a valid range
  if (k > static_cast<int>(trainVectors.size())) {
    cerr << "Error: Invalid k value. It cannot exceed the size of the training "
            "vectors."
         << endl;
    return 1;
  }

  // Perform nearest neighbor search
  VectorDataset result = knearestneighbor(trainVectors, testVectors, k);

  
 // Display the result
  
  for (int i = 0,t=0; i < result.size(); ++i) {
    if(i%k==0){
      printf("For ");
      testVectors[t].printVector();
      printf("Closet %d vectors are:\n",k);
      t++;
    }
    DataVector neighbor = result.getDataVector(i);
    neighbor.printVector();
  }
  // Stop the timer after displaying neighbors
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Nearest neighbor search took " << duration.count() << " milliseconds." << endl;

  return 0;
}

VectorDataset readDatasetFromFile(const string &filename) {
  VectorDataset data;

  ifstream file(filename);
  string line = "";

  if (file.is_open()) {
    // Read lines from the CSV file
    while (getline(file, line)) {
      vector<double> row;
      stringstream iss(line);

      // Parse all components from a single line
      double vect_component;
      while (iss >> vect_component) {
        // cout << "vect: " << vect_component << "\n";
        row.push_back(vect_component);

        // Check for the comma to move to the next component
        if (iss.peek() == ',')
          iss.ignore();
      }

      if (!row.empty()) {
        DataVector dataVector;
        dataVector.setValues(row);
        data.addDataVector(dataVector);
      }
    }

    file.close();
  } else {
    cerr << "Unable to open file: " << filename << endl;
  }

  return data;
}

// Nearest neighbor search function
VectorDataset knearestneighbor(const vector<DataVector> &trainVectors,
                               const vector<DataVector> &testVectors, int k) {
  VectorDataset result;

  // Check if the datasets are empty or if k is greater than the training
  // dataset size
  if (trainVectors.size() == 0 || testVectors.size() == 0 ||
      k > int(trainVectors.size())) {
    cerr << "Error: Invalid training or test dataset or k value." << endl;
    return result;
  }

  // Iterate through test vectors
  for (const DataVector &testVector : testVectors) {
    // Vector to store distances and corresponding indices
    vector<pair<double, int>> distances;

    // Calculate distances between the test vector and each vector in the
    // training dataset
    for (int j = 0; j < trainVectors.size(); ++j) {
      double distance = testVector.dist(trainVectors[j]);
      distances.push_back(make_pair(distance, j));
    }

    // Sort distances in ascending order
    sort(distances.begin(), distances.end());

    // Select the top k vectors with the smallest distances
    for (int m = 0; m < k; ++m) {
      int index = distances[m].second;
      result.addDataVector(trainVectors[index]);
    }
  }

  return result;
}
