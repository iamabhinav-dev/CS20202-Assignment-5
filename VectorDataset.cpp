#include <iostream>
#include<bits/stdc++.h>
#include "VectorDataset.h"
using namespace std;
VectorDataset::VectorDataset() {

}

VectorDataset::~VectorDataset() {
    dataset.clear();
}

void VectorDataset::addDataVector(const DataVector& dataVector) {
    dataset.push_back(dataVector);
}
const vector<DataVector>& VectorDataset::getDataset(){
    return dataset;
}
DataVector VectorDataset::getDataVector(int index) const {
    return dataset[index];
}

int VectorDataset::size() const {
    return dataset.size();
}

VectorDataset VectorDataset::readDatasetFromFile(const string &filename) {
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
VectorDataset VectorDataset::knearestneighbor(const VectorDataset &trainVectors, int k) {
  VectorDataset result;

  // Check if the datasets are empty or if k is greater than the training
  // dataset size
  if (trainVectors.size() == 0 || dataset.size() == 0 ||
      k > int(trainVectors.size())) {
    cerr << "Error: Invalid training or test dataset or k value." << endl;
    return result;
  }

  // Iterate through test vectors
  for (const DataVector &dataset : dataset) {
    // Vector to store distances and corresponding indices
    vector<pair<double, int>> distances;

    // Calculate distances between the test vector and each vector in the
    // training dataset
    for (int j = 0; j < trainVectors.size(); ++j) {
      double distance = dataset.dist(trainVectors.getDataVector(j));
      distances.push_back(make_pair(distance, j));
    }

    // Sort distances in ascending order
    sort(distances.begin(), distances.end());

    // Select the top k vectors with the smallest distances
    for (int m = 0; m < k; ++m) {
      int index = distances[m].second;
      result.addDataVector(trainVectors.getDataVector(index));
    }
  }

  return result;
}