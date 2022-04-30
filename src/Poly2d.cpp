#include "Poly2d.h"
#include "QuickHull.hpp"

using namespace std;
using namespace Eigen;

Poly2d Poly2d::ConvexUnion(Poly2d &p) {
    
    vector<Vector2d> v;
    vector<Vector2d>::iterator it;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it) v.push_back(*it);
    for(it = p.mPoints.begin(); it != p.mPoints.end(); ++it) v.push_back(*it);
    
    // simplify and return
    Poly2d m = Poly2d(v); m.Simplify();
    return m;
    
}

Poly2d Poly2d::MinkowskiSum(Poly2d &p) {
    
    // computing Minkowski sum
    vector<Vector2d> v;
    vector<Vector2d>::iterator it1, it2;
    for(it1 = this->mPoints.begin(); it1 != this->mPoints.end(); ++it1) {
        for(it2 = p.mPoints.begin(); it2 != p.mPoints.end(); ++it2) {
            v.push_back(*it1 + *it2);
        }
    }
    
    // simplify and return
    Poly2d m = Poly2d(v); m.Simplify();
    return m;
    
}

void Poly2d::Simplify() {
    
    // from Eigen to Quickhull
    vector<Vector2d>::iterator it;
    vector<quickhull::Vector3<double>> allpoints;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it) allpoints.push_back( quickhull::Vector3<double>((*it)(0), (*it)(1), .0) );
    
    // Compute convex hull
    quickhull::QuickHull<double> qh;
    auto hull = qh.getConvexHull(allpoints, true, false);
    const auto& indexBuffer = hull.getIndexBuffer();
    const auto& vertexBuffer = hull.getVertexBuffer();
    
    // Extract vertices
    vector<Vector2d> vsimple;
    for(size_t i = 0; i < indexBuffer.size(); ++i)
        vsimple.push_back( Vector2d(vertexBuffer[indexBuffer[i]].x, vertexBuffer[indexBuffer[i]].y) );
    this->mPoints = vsimple;
    
}

Poly2d Poly2d::Intersection(Poly2d &p) {
    
    // to be done
    vector<Vector2d> v;
    Poly2d m = Poly2d(v); m.Simplify();
    return m;
    
}
