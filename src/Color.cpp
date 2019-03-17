#include "../include/Color.h"

Color::Color(){}

Color::Color(GLfloat r, GLfloat g, GLfloat b):red(r),green(g),blue(b){}

Color::~Color(){}

void Color::set_red(GLfloat r){
    red = r;
}

void Color::set_green(GLfloat g){
    green = g;
}

void Color::set_blue(GLfloat b){
    blue = b;
}

GLfloat Color::get_red() const{
    return red;
}

GLfloat Color::get_green() const{
    return green;
}

GLfloat Color::get_blue() const{
    return blue;
}

void Color::set_color(GLfloat r, GLfloat g, GLfloat b) {
    red = r;
    green = g;
    blue = b;
}

std::ostream & operator << (std::ostream &out, const Color &c){
    out << "Color:" << c.get_red() << " "<< c.get_green() << " "<< c.get_blue() << std::endl;
    return out;
}
