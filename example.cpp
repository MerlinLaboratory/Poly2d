#include "Poly2d.h"

int main(void) {
    
    // entering two new polygons
    std::vector<Eigen::Vector2d> triangle{ Eigen::Vector2d(0.0,0.0), Eigen::Vector2d(2.0,0.0), Eigen::Vector2d(0.0,1.0)},
                                 square{ Eigen::Vector2d(1.0,-1.0), Eigen::Vector2d(3.0,-1.0), Eigen::Vector2d(3.0,1.0), Eigen::Vector2d(1.0,1.0)};
    Poly2d t = Poly2d(triangle), s = Poly2d(square);
    
    // discretized circle
    std::vector<Eigen::Vector2d> circle;
    for(double theta = 0; theta < 2*M_PI; theta += .05 * M_PI) circle.push_back(.1 * Eigen::Vector2d(cos(theta), sin(theta)));
    Poly2d c = Poly2d(circle);
    
    // print out given polygons
    std::cout << t << std::endl << std::endl << s << std::endl << std::endl << c << std::endl << std::endl;
      
    // testing
    Poly2d x = t.MinkowskiSum(c); std::cout << x << std::endl << std::endl;
    Poly2d y = s.Intersection(x); std::cout << y << std::endl << std::endl;
    
    return 0;
    
}
