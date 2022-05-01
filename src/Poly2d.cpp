#include "Poly2d.h"
#include "QuickHull.hpp"

using namespace std;
using namespace Eigen;

#define SIGNED_AREA(x,y,z) (x(0)-z(0))*(y(1)-z(1))-(x(1)-z(1))*(y(0)-z(0))

bool sort_fun(pair<Vector2d, double> &x, pair<Vector2d, double> &y);
bool segm_intersection(Vector2d &a, Vector2d &b, Vector2d &c, Vector2d &d, Vector2d* pinter);

Poly2d::Poly2d(std::vector<Eigen::Vector2d>& points) {
    this->mPoints = points;
    this->Simplify();
}

bool Poly2d::IsInside(Vector2d& point) {
    
    bool c = false;
    size_t i, j, nvert = this->mPoints.size();
    for(i = 0, j = nvert - 1; i < nvert; j = i++) {
        if( ((this->mPoints[i](1) >= point(1)) != (this->mPoints[j](1) >= point(1))) &&
            (point(0) <= (this->mPoints[j](0) - this->mPoints[i](0)) * (point(1) - this->mPoints[i](1)) / (this->mPoints[j](1) - this->mPoints[i](1)) + this->mPoints[i](0)) )
            c = !c;
    }
    return c;
    
}

double Poly2d::Area() {
    
    double area = .0;
    vector<Vector2d>::iterator it;
    size_t nverts = this->mPoints.size();
    for(size_t i = 0; i < nverts; ++i) {
        int j = (i+1) % nverts;
        area += this->mPoints[i](0) * this->mPoints[j](1);
        area -= this->mPoints[i](1) * this->mPoints[j](0);
    }
    return 0.5 * fabs(area);
    
}

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

Poly2d Poly2d::MinkowskiDiff(Poly2d &p) {
    
    // computing Minkowski sum
    vector<Vector2d> v;
    vector<Vector2d>::iterator it1, it2;
    for(it1 = this->mPoints.begin(); it1 != this->mPoints.end(); ++it1) {
        for(it2 = p.mPoints.begin(); it2 != p.mPoints.end(); ++it2) {
            v.push_back(*it1 - *it2);
        }
    }
    
    // simplify and return
    Poly2d m = Poly2d(v); m.Simplify();
    return m;
    
}

Poly2d Poly2d::Intersection(Poly2d &p) {
    
    vector<Vector2d> v;
    vector<Vector2d>::iterator it;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it) if(p.IsInside(*it)) v.push_back(*it);
    for(it = p.mPoints.begin(); it != p.mPoints.end(); ++it) if(this->IsInside(*it)) v.push_back(*it);
    Vector2d p0, p1, p2, p3, pinter;
    size_t nthis = this->mPoints.size(), np = p.mPoints.size();
    for(size_t i = 0; i < this->mPoints.size(); ++i) {
        p0 = this->mPoints[i];
        p1 = this->mPoints[(i+1) % nthis];
        for(size_t j = 0; j < p.mPoints.size(); ++j) {
            p2 = p.mPoints[j];
            p3 = p.mPoints[(j+1) % np];
            if(segm_intersection(p0, p1, p2, p3, &pinter)) v.push_back(pinter);
        }
    }
    
    // order vertices (no need to simplify) and return
    Poly2d m = Poly2d(v); m.OrderVertices();
    return m;

}

ostream& operator<<(ostream& os, Poly2d &p) {
    
    os << "Polygon:";
    vector<Vector2d>::iterator it;
    for(it = p.mPoints.begin(); it != p.mPoints.end(); ++it) os << " " << (*it)(0) << ", " << (*it)(1) << ";";
    return os;
    
}


void Poly2d::Simplify() {
    
    // from Eigen to Quickhull
    vector<Vector2d>::iterator it;
    vector<quickhull::Vector3<double>> allpoints;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it) allpoints.push_back( quickhull::Vector3<double>((*it)(0), (*it)(1), .0) );
    
    // Compute convex hull
    quickhull::QuickHull<double> qh;
    auto hull = qh.getConvexHull(allpoints, true, false);
    const auto& vertexBuffer = hull.getVertexBuffer();
    
    // Extract vertices and order them counter-clockwise
    vector<Vector2d> vsimple;
    for(size_t i = 0; i < vertexBuffer.size(); ++i) vsimple.push_back( Vector2d(vertexBuffer[i].x, vertexBuffer[i].y) );
    this->mPoints = vsimple;
    this->OrderVertices();
    
}

void Poly2d::OrderVertices() {
    
    if(this->mPoints.empty()) return;
    
    // compute centroid
    Vector2d c = Vector2d::Zero();
    vector<Vector2d>::iterator it;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it) c += *it;
    c /= double(this->mPoints.size());
    
    // order vertices counter-clockwise
    vector<pair<Vector2d, double> > to_order;
    for(it = this->mPoints.begin(); it != this->mPoints.end(); ++it)
        to_order.push_back(std::make_pair(*it, atan2((*it)(1)-c(1), (*it)(0)-c(0))));
    sort(to_order.begin(), to_order.end(), sort_fun);
    this->mPoints.clear();
    
    // extract vertices
    vector<pair<Vector2d, double> >::iterator its;
    for(its = to_order.begin(); its != to_order.end(); ++its) this->mPoints.push_back(its->first);
    
}

bool sort_fun(pair<Vector2d, double> &x, pair<Vector2d, double> &y) {
    
    return (x.second > y.second);
    
}

bool segm_intersection(Vector2d &a, Vector2d &b, Vector2d &c, Vector2d &d, Vector2d* pinter) {
    
    double a1 = SIGNED_AREA(a, b, d); // Compute winding of abd (+ or -)
    double a2 = SIGNED_AREA(a, b, c); // To intersect, must have sign opposite of a1
    if (a1 * a2 < 0.0f) {
        double a3 = SIGNED_AREA(c, d, a); // Compute winding of cda (+ or -)
        double a4 = a3 + a2 - a1;
        if (a3 * a4 < .0) {
            double t = a3 / (a3 - a4);
            *pinter = a + t * (b - a);
            return true;
        }
    }
    
    // Segments not intersecting (or collinear)
    return false;
    
}
