/**
 * @file VectorDataset.h
 * @brief Defines the VectorDataset class, representing a collection of DataVectors with associated operations.
 *
 * The VectorDataset class serves as a container for managing and organizing a collection of DataVectors.
 * Each DataVector within the dataset represents a vector of double values. This class provides functionalities
 * for adding, accessing, and analyzing DataVectors within the dataset. It is designed to facilitate the storage
 * and retrieval of vectors in applications such as machine learning, data analysis, and numerical simulations.
 * The VectorDataset class encapsulates common operations such as adding a DataVector to the dataset, retrieving
 * a DataVector at a specific index, and obtaining the size of the dataset. Leveraging the capabilities of the
 * DataVector class, VectorDataset provides a flexible and extensible structure for handling datasets of varying
 * dimensions and complexities.
 *
 * @author Abhinav Kumar Singh
 */



#ifndef VECTORDATASET_H
#define VECTORDATASET_H

#include "DataVector.h"
#include <vector>

class VectorDataset {
private:
    std::vector<DataVector> dataset;

public:
    // Constructors and Destructor
    VectorDataset();
    ~VectorDataset();

    // Add DataVector to the dataset
    void addDataVector(const DataVector& dataVector);
    const vector<DataVector>& getDataset();
    // Get DataVector at a specific index
    DataVector getDataVector(int index) const;

    // Get the size of the dataset
    int size() const;
    VectorDataset readDatasetFromFile(const string &filename);
    VectorDataset knearestneighbor(const VectorDataset &trainVectors, int k);
};

#endif // VECTORDATASET_H
