#ifndef Color_H_
#define Color_H_

#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

class Color {
public:
    Color();

    Color(GLfloat r, GLfloat g, GLfloat b);

    ~Color();

    void set_red(GLfloat r);

    void set_green(GLfloat g);

    void set_blue(GLfloat b);

    GLfloat get_red() const;

    GLfloat get_green() const;

    GLfloat get_blue() const;

    void set_color(GLfloat r, GLfloat g, GLfloat b);

    friend std::ostream & operator << (std::ostream &out, const Color &c);
private:
    GLfloat red, green, blue;
};

#endif