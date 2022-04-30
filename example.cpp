#include "Poly2d.h"

int main(void) {
    
    // entering two new polygons
    std::vector<Eigen::Vector2d> triangle{ Eigen::Vector2d(0.0,0.0), Eigen::Vector2d(2.0,0.0), Eigen::Vector2d(0.0,1.0)},
                                 square{ Eigen::Vector2d(1.0,-1.0), Eigen::Vector2d(2.0,-1.0), Eigen::Vector2d(2.0,1.0), Eigen::Vector2d(1.0,1.0)};
    Poly2d t = Poly2d(triangle), s = Poly2d(square);
      
    // testing
    Poly2d x = s.Intersection(t);
    std::cout << x << std::endl;
    
    return 0;
    
}
