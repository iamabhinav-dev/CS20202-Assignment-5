#include "DataVector.h"
#include <cmath>
using namespace std;


// Constructor
DataVector::DataVector(int dimension) {
  v.resize(dimension, 0);
 // printf("DataVector Intialised \n");
}
// Destructor
DataVector::~DataVector() {
  v.clear();
 // printf("Destructor called\n");
}
// Copy Constructor
DataVector::DataVector(const DataVector &other) {
  if (this != &other) {
    v = other.v;
   // printf("Copy Constructor called\n");
  }
  // return this;
}
// Copy assignment operator
DataVector &DataVector::operator=(const DataVector &other) {
  if (this != &other) {
    v = other.v;
   // printf("Copy assignment operator called\n");
  }
  return *this;
}

// addtion operator
DataVector DataVector::operator+(const DataVector &other) {
  int size = min(other.v.size(), v.size());
  DataVector ans(max(other.v.size(), v.size()));
  for (int i = 0; i < max(other.v.size(), v.size()); i++) {
    // adding upto the min of dimensions of both vectors
    if (i < size) {
      ans.v[i] = v[i] + other.v[i];
    }
    // assigning the left ones directly
    else {
      if (size == int(other.v.size())) {
        ans.v[i] = v[i];
      } else
        ans.v[i] = other.v[i];
    }
  }
  return ans;
}
// subtraction opearator
DataVector DataVector::operator-(const DataVector &other) const{
    int size = std::min(other.v.size(), v.size());
    DataVector ans(std::max(other.v.size(), v.size()));

    for (int i = 0; i < int(ans.v.size()); i++) {
        if (i < size) {
            ans.v[i] = v[i] - other.v[i];
        } else {
            // If one vector is longer, subtract the remaining elements or just copy them
            ans.v[i] = (v.size() == ans.v.size()) ? v[i] : -other.v[i];
        }
    }

    return ans;
}


// dot product operator
double DataVector::operator *(const DataVector &other)const {
  double ans = 0;
  int size = min(other.v.size(), v.size());
  for (int i = 0; i < size; i++) {
    // element wise multiplication
    ans += (other.v[i] * v[i]);
  }

  return ans;
}
// size of vector
double DataVector::norm() { return sqrt(*this * *this); }
// distance between two vector

double DataVector::dist(const DataVector &other) const
 {
  DataVector ans = (*this) - other;

  return (ans.norm());
}
vector<double> DataVector::getVector()const{
  return v;
}
void DataVector::setValues(const vector<double>& values) {
    v = values;
}
void DataVector::printVector() {
  printf(" Vector reprsentation in Component form:(%.2f,",v[0]);
  for (int i = 1; i < int(v.size())-1; i++) {
    printf("%.2f,", v[i]);
  }
  printf("%.2f)\n\n",v[v.size()-1]);
}
int DataVector::size(){
  return v.size();
}
