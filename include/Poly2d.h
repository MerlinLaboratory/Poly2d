#ifndef __H_INCLUDE_POLY2D
#define __H_INCLUDE_POLY2D

#include <iostream>
#include <vector>
#include <Eigen/Core>

class Poly2d {
public:
// constructors
    Poly2d() { };
    Poly2d(std::vector<Eigen::Vector2d>& points);
// methods
    bool IsInside(Eigen::Vector2d& p);
    bool IsEmpty();
    double Area();
// operations
    Poly2d ConvexUnion(Poly2d &p);
    Poly2d MinkowskiSum(Poly2d &p);
    Poly2d MinkowskiDiff(Poly2d &p);
    Poly2d Intersection(Poly2d &p);
// operators
    friend std::ostream& operator<<(std::ostream& os, Poly2d &p);
private:
// convex hull
    void Simplify();
    void OrderVertices();
// data
    std::vector<Eigen::Vector2d> mPoints;
};

#endif
