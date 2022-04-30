# Poly2d

Library to manage 2D convex polygons.

Requires:
 - Eigen (https://eigen.tuxfamily.org)
 - Quickhull (https://github.com/akuukka/quickhull)

Example of usage:

```
  // entering two new polygons
  std::vector<Eigen::Vector2d> triangle{ Eigen::Vector2d(0.0,0.0), Eigen::Vector2d(1.0,0.0), Eigen::Vector2d(0.0,1.0)},
                               square{ Eigen::Vector2d(0.0,0.0), Eigen::Vector2d(1.0,0.0), Eigen::Vector2d(1.0,1.0), Eigen::Vector2d(0.0,1.0)}; 
  Poly2d t = Poly2d(triangle), s = Poly2d(square);
  
  // testing basic operations
  Poly2d u = square.ConvexUnion(triangle), square.MinkowskiSum(triangle);
```
