#ifndef TREEINDEX_H
#define TREEINDEX_H

#include "DataVector.h"
#include "VectorDataset.h"
using namespace std;

// Base class TreeIndex
class TreeIndex
{
protected:
    VectorDataset dataset;
    TreeIndex(); // Protected constructor

public:
    static TreeIndex &GetInstance(); // Static function to get a singleton instance
    void addData(const DataVector &dataVector); // Add data to the dataset
    void removeData(const DataVector &dataVector); // Remove data from the dataset 
    ~TreeIndex(); // Destructor
};

// Derived class KDTreeIndex
class KDTreeIndex : public TreeIndex
{
    KDTreeIndex(); // Private constructor
    double median;
    int axis;
    KDTreeIndex *LEFT;
    KDTreeIndex *RIGHT;

public:
    ~KDTreeIndex(); // Destructor
    static KDTreeIndex &GetInstance(); // Static function to get a singleton instance
    KDTreeIndex *BuildTree(); // Build KD-Tree
    pair<VectorDataset, VectorDataset> ChooseRule(); // Choose the splitting rule for KD-Tree
    vector<DataVector> SearchFunc(const VectorDataset train, int k, const DataVector test); // Search for k-nearest neighbors in KD-Tree
    int getmaxSpreadAxis(); // Find the axis with maximum spread
    double getMedian() const; // Getter for the median
    int getAxis() const; // Getter for the axis
    double Hyperplane(const DataVector& query, const KDTreeIndex* node); // Calculate the distance from a hyperplane
};

// Derived class RPTreeIndex
class RPTreeIndex : public TreeIndex
{
private:
    struct node{
        vector<DataVector> v;
        node* LEFT;
        node* RIGHT;
        double median;
        vector<double> axis;
    };
    node* root;
    static RPTreeIndex* instance; // Singleton instance
    vector<double> unitdir(size_t dimension); // Generate a random unit direction vector
    int randval(int k); // Generate a random value within a given range
    node* buildTree(VectorDataset &points); // Build RP-Tree
    node* buildTree(vector<DataVector>::iterator begin, vector<DataVector>::iterator end); // Recursive function to build RP-Tree
    vector<DataVector> SearchFunc(DataVector &points, node* nodepoint, int k); // Search for k-nearest neighbors in RP-Tree
    RPTreeIndex() {} // Private constructor
    ~RPTreeIndex() {} // Destructor

public:
    static RPTreeIndex &GetInstance(); // Static function to get a singleton instance
    void maketree(VectorDataset &points); // Build RP-Tree using the provided dataset
    vector<DataVector> searchquery(DataVector &point, int k); // Search for k-nearest neighbors in RP-Tree for a given query point
};

#endif // TREEINDEX_H
