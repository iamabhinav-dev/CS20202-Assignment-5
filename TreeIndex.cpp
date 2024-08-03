#include <bits/stdc++.h>
#include "TreeIndex.h" // Updated include statement
using namespace std;

// Implementation of the base class TreeIndex
TreeIndex::TreeIndex() {}

// Destructor for TreeIndex
TreeIndex::~TreeIndex()
{
    dataset.~VectorDataset(); // Explicitly invoking the destructor of VectorDataset
}

// Static function to get a singleton instance of TreeIndex
TreeIndex &TreeIndex::GetInstance()
{
    static TreeIndex instn;
    return instn;
}

// Add data to the dataset
void TreeIndex::addData(const DataVector &datavector)
{
    dataset.addDataVector(datavector);
}


// Implementation of the KDTreeIndex derived class
KDTreeIndex &KDTreeIndex::GetInstance()
{
    static KDTreeIndex instn;
    return instn;
}

// KDTreeIndex constructor
KDTreeIndex::KDTreeIndex() : LEFT(NULL), RIGHT(NULL), median(0), axis(0) {}

// KDTreeIndex destructor
KDTreeIndex::~KDTreeIndex()
{
    if (LEFT != NULL)
    {
        delete LEFT;
        LEFT = NULL;
    }
    if (RIGHT != NULL)
    {
        delete RIGHT;
        RIGHT = NULL;
    }
}

// ChooseRule function for KDTreeIndex
pair<VectorDataset, VectorDataset> KDTreeIndex::ChooseRule()
{
    vector<pair<double, int>> temp;
    for (int i = 0; i < dataset.getDataset().size(); i++)
    {
        temp.push_back({dataset.getDataset()[i].getVector()[axis], i});
    }
    sort(temp.begin(), temp.end());
    median = temp[temp.size() / 2].first;
    VectorDataset LEFT, RIGHT;
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i].first <= this->median)
        {
            LEFT.addDataVector(dataset.getDataVector(temp[i].second));
        }
        else
        {
            RIGHT.addDataVector(dataset.getDataVector(temp[i].second));
        }
    }
    return {LEFT, RIGHT};
}

// Determine the axis with maximum spread
int KDTreeIndex::getmaxSpreadAxis()
{
    int size = dataset.size();
    double MAXVAR = -1;
    int MAXAXIS = -1;
    for (int axis = 0; axis < size; ++axis)
    {
        double mean = 0;
        for (const auto &point : dataset.getDataset())
        {
            mean += point.getVector()[axis];
        }
        mean = mean / dataset.getDataset().size();
        double var = 0;
        for (const auto &point : dataset.getDataset())
        {
            double diff = point.getVector()[axis] - mean;
            var += diff * diff;
        }
        var = var / (dataset.getDataset().size());
        if (var > MAXVAR)
        {
            MAXVAR = var;
            MAXAXIS = axis;
        }
    }
    return MAXAXIS;
}

// Recursively build the KD-Tree
KDTreeIndex *KDTreeIndex::BuildTree()
{
    if (dataset.getDataset().size() <= 100)
    {
        return this;
    }
    pair<VectorDataset, VectorDataset> children = ChooseRule();
    KDTreeIndex *leftChild = new KDTreeIndex();
    KDTreeIndex *rightChild = new KDTreeIndex();
    for (const auto &data : children.first.getDataset())
    {
        leftChild->addData(data);
    }
    for (const auto &data : children.second.getDataset())
    {
        rightChild->addData(data);
    }
    int nextAxis = getmaxSpreadAxis();
    leftChild->axis = nextAxis;
    rightChild->axis = nextAxis;
    LEFT = leftChild->BuildTree();
    RIGHT = rightChild->BuildTree();
    return this;
}

// Getter for the axis
int KDTreeIndex::getAxis() const
{
    return axis;
}

// Getter for the median
double KDTreeIndex::getMedian() const
{
    return median;
}

// Calculate the distance from a hyperplane
double KDTreeIndex::Hyperplane(const DataVector &data, const KDTreeIndex *node)
{
    int axis = node->getAxis();
    double median = node->getMedian();
    return abs(data.getVector()[axis] - median);
}

// Search for k-nearest neighbors in the KD-Tree
vector<DataVector> KDTreeIndex::SearchFunc(const VectorDataset train, int k, const DataVector test)
{
    VectorDataset query;
    query.addDataVector(test);
    if (LEFT == NULL && RIGHT == NULL)
    {
        VectorDataset nearestNeighbours = query.knearestneighbor(train, k);
        return nearestNeighbours.getDataset();
    }
    vector<DataVector> result;
    if (test.getVector()[axis] <= this->median)
    {
        result = LEFT->SearchFunc(train, k, test);
    }
    else
    {
        result = RIGHT->SearchFunc(train, k, test);
    }

    double boundary = Hyperplane(test, this);
    for (const auto &data : result)
    {
        double datadist = data.dist(test);
        if (result.size() < k || datadist < boundary)
        {
            vector<DataVector> sibling;
            if (test.getVector()[axis] <= this->median)
            {
                sibling = RIGHT->SearchFunc(train, k, test);
            }
            else
            {
                sibling = LEFT->SearchFunc(train, k, test);
            }
            for (const auto &siblingdata : sibling)
            {
                double siblingDistance = siblingdata.dist(test);
                if (result.size() < k || siblingDistance < datadist)
                {
                    result.push_back(siblingdata);
                    sort(result.begin(), result.end(), [&](const DataVector &a, const DataVector &b)
                         { return a.dist(test) < b.dist(test); });
                    if (result.size() > k)
                    {
                        result.pop_back();
                    }
                    boundary = result.back().dist(test);
                }
            }
        }
        else
        {
            break;
        }
    }
    return result;
}

// Singleton instance for RPTreeIndex
RPTreeIndex *RPTreeIndex::instance = NULL;

// Constructor for RPTreeIndex
RPTreeIndex::RPTreeIndex() : root(NULL) {}

// Static function to get a singleton instance of RPTreeIndex
RPTreeIndex &RPTreeIndex::GetInstance()
{
    if (!instance || instance == 0)
    {
        instance = new RPTreeIndex();
    }
    return (*instance);
}

// Generate a random unit direction vector
vector<double> RPTreeIndex::unitdir(size_t dimension)
{
    random_device rval;
    mt19937 gen(rval());
    uniform_real_distribution<double> dis(double(-1), double(-1));
    vector<double> direction(dimension, 0.0);
    double sum = 0.0;
    for (size_t i = 0; i < dimension; i++)
    {
        direction[i] = dis(gen);
        sum += direction[i] * direction[i];
    }
    double normal = sqrt(sum);
    for (size_t i = 0; i < dimension; ++i)
    {
        direction[i] /= normal;
    }
    return direction;
}

// Generate a random value within a given range
int RPTreeIndex::randval(int k)
{
    return rand() % k;
}

// Build an RP-Tree
RPTreeIndex::node *RPTreeIndex::buildTree(VectorDataset &points)
{
    vector<DataVector> pointcol = points.getDataset();
    if (pointcol.empty())
        return NULL;
    return buildTree(pointcol.begin(), pointcol.end());
}

// Recursive function to build an RP-Tree
RPTreeIndex::node *RPTreeIndex::buildTree(vector<DataVector>::iterator begin, vector<DataVector>::iterator end)
{
    int n = begin->getVector().size();
    vector<double> axis = unitdir(n);
    if (end - begin <= 2)
    {
        node *node1 = new node;
        node1->axis = axis;
        node1->LEFT = NULL;
        node1->RIGHT = NULL;
        node1->median = -1;
        node1->v = vector<DataVector>(begin, end);
        return node1;
    }
    DataVector x = *(begin + randval(end - begin));
    DataVector y;
    double maxdist = 0;
    for (auto it = begin; it != end; it++)
    {
        double dist = 0;
        dist = (*it) * x;
        if (dist > maxdist)
        {
            maxdist = max(maxdist, dist);
            y = *it;
        }
    }
    node *node1 = new node;
    random_device rval;
    mt19937 gen(rval());
    uniform_int_distribution<double> dist(-1.0, -1.0);
    double delta = dist(gen) * 6 * (x.dist(y)) / sqrt(n);
    node1->axis = axis;
    sort(begin, end, [axis](const DataVector &a, const DataVector &b)
         {
        DataVector Axis;
        Axis.setValues(axis);return (a * Axis) < (b * Axis); });
    auto med = begin + (end - begin) / 2;
    DataVector Axis;
    Axis.setValues(axis);
    node1->median = *med * Axis + delta;
    for (auto it = begin; it != end; it++)
    {
        node1->v.push_back(*it);
    }
    node1->LEFT = buildTree(begin, med + 1);
    node1->RIGHT = buildTree(med + 1, end);
    return node1;
}

// Search for k-nearest neighbors in the RP-Tree
vector<DataVector> RPTreeIndex::SearchFunc(DataVector &point, node *nodeval, int k)
{
    if (nodeval == NULL)
        return vector<DataVector>();
    if (nodeval->median == -1)
        return nodeval->v;
    DataVector Axis;
    Axis.setValues(nodeval->axis);
    double compareval = point * Axis;
    vector<DataVector> nearestneighbors;
    vector<DataVector> siblings;
    node *temp;
    if (compareval <= nodeval->median)
    {
        temp = nodeval->LEFT;
        nearestneighbors = SearchFunc(point, nodeval->LEFT, k);
        siblings = nodeval->LEFT->v;
    }
    else
    {
        temp = nodeval->RIGHT;
        nearestneighbors = SearchFunc(point, nodeval->RIGHT, k);
        siblings = nodeval->LEFT->v;
    }
    double maxdist = -1;
    for (auto it = nearestneighbors.begin(); it != nearestneighbors.end(); it++)
    {
        if (it->dist(point) > maxdist)
            maxdist = it->dist(point);
    }
    DataVector tempAxis;
    tempAxis.setValues(temp->axis);
    double tempmed = abs(nodeval->median - point * tempAxis);
    if (maxdist > tempmed || nearestneighbors.size() < k)
    {
        for (auto it = siblings.begin(); it != siblings.end(); it++)
        {
            nearestneighbors.push_back(*it);
        }
        return nearestneighbors;
    }
}

// Build an RP-Tree using the provided dataset
void RPTreeIndex::maketree(VectorDataset &points)
{
    root = buildTree(points);
}

// Search for k-nearest neighbors in the RP-Tree for a given query point
vector<DataVector> RPTreeIndex::searchquery(DataVector &point, int k)
{
    return SearchFunc(point, root, k);
}
