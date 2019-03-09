#include "Point.h"

Point::Point(){}

Point::Point(GLfloat _x,GLfloat _y,GLfloat _z){
    x = _x;
    y = _y;
    z = _z;
}

Point::~Point(){}

GLfloat Point::getX() const {
    return x;
}

GLfloat Point::getY() const {
    return y;
}

GLfloat Point::getZ() const {
    return z;
}

void Point::setX(float _x) {
    x = _x;
}
void Point::setY(float _y) {
    y = _y;
}
void Point::setZ(float _z) {
    z = _z;
}

void Point::setXY(float _x,float _y) {
    x = _x;
    y = _y;
}

void Point::set_coord(GLfloat _x,GLfloat _y,GLfloat _z) {
    x = _x;
    y = _y;
    z = _z;
}

GLfloat Point::get_distance(const Point &other) const {
    return sqrt(pow(this->x-other.x,2)+pow(this->y-other.y,2)+pow(this->z-other.z,2));
}

void Point::normalize(GLfloat norm){
    x = x/norm;
    y = y/norm;
    z = z/norm;
}

std::istream & operator >> (std::istream &in, Point &p){
    in >> p.x >> p.y >> p.z;
    return in;
}

std::ostream & operator << (std::ostream &out, const Point &p){
    out << "Point:" << p.x << " "<< p.y << " "<< p.z << std::endl;
    return out;
}

Point operator + (Point const& obj1,Point const& obj2){
    return Point(obj1.getX() + obj2.getX(),obj1.getY() + obj2.getY(),obj1.getZ() + obj2.getZ());
}

Point operator - (Point const& obj1,Point const& obj2){
    return Point(obj1.getX() - obj2.getX(),obj1.getY() - obj2.getY(),obj1.getZ() - obj2.getZ());
}
