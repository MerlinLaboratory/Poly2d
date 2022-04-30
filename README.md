# Poly2d

Library to manage 2D convex polygons.

Requires:
 - Eigen (https://eigen.tuxfamily.org)
 - Quickhull (https://github.com/akuukka/quickhull)

Example of usage:

```
    // entering two new polygons
    std::vector<Eigen::Vector2d> triangle{ Eigen::Vector2d(0.0,0.0), Eigen::Vector2d(2.0,0.0), Eigen::Vector2d(0.0,1.0)},
                                 square{ Eigen::Vector2d(1.0,-1.0), Eigen::Vector2d(2.0,-1.0), Eigen::Vector2d(2.0,1.0), Eigen::Vector2d(1.0,1.0)};
    Poly2d t = Poly2d(triangle), s = Poly2d(square);
      
    // testing
    Poly2d x = s.Intersection(t);
    std::cout << x << std::endl;
```
![example](https://user-images.githubusercontent.com/60401657/166119387-a5c3bd0b-702b-4b5d-bd67-91833c6ecdaa.png)
