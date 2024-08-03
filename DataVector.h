/**
 * @file DataVector.h
 * @brief Defines the DataVector class, representing a vector of double values with associated operations.
 *
 * The DataVector class encapsulates the concept of a mathematical vector in Euclidean space,
 * where each component is a double precision floating-point value. This class provides essential
 * functionalities for the creation, manipulation, and analysis of vectors. Operations such as vector
 * addition, subtraction, dot product calculation, size (norm) computation, and distance calculation
 * between two vectors are supported. Additionally, the class allows for efficient handling of vectors
 * with varying dimensions, offering flexibility in various mathematical applications.
 *
 * @author Abhinav Kumar Singh
                                                                                                                               
 */


#include <vector>
#include <cstdio>
class DataVector {

  std::vector<double> v;

public:
  // Constructor
  DataVector(int dimension = 0);
  // Destructor
  ~DataVector();
  // Copy Constructor
  DataVector(const DataVector &other);
  // Copy assignment operator
  DataVector &operator=(const DataVector &other);
  // addtion operator
  DataVector operator+(const DataVector &other);
  // subtraction opearator
  DataVector operator-(const DataVector &other)const;
  // dot product operator
  double operator*(const DataVector &other)const;
  // size of vector
  double norm();
  // distance between two vector
  double dist(const DataVector &other) const;
  vector<double> getVector()const;
  //set values of datavector v by taking an vector as input
  void setValues(const std:: vector<double>& values);
  //print the values of vector as component form
  void printVector();
int  size();
};
