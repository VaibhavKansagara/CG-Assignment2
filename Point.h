#ifndef Point_H_
#define Point_H_

#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

class Point{
public:
    Point();

    Point(GLfloat _x,GLfloat _y,GLfloat _z = 0.0);

    ~Point();

    GLfloat getX() const;

    GLfloat getY() const;

    GLfloat getZ() const;

    void setX(float _x);

    void setY(float _y);

    void setZ(float _z);

    void setXY(float _x,float _y);

    void set_coord(GLfloat _x,GLfloat _y,GLfloat _z = 0.0);

    GLfloat get_distance(const Point &other) const;

    void normalize(GLfloat norm);

    friend std::istream & operator >> (std::istream &in, Point &p);

    friend std::ostream & operator << (std::ostream &out, const Point &p);

    friend Point operator + (Point const& obj1,Point const& obj2);
private:
    GLfloat x,y,z;
};

#endif