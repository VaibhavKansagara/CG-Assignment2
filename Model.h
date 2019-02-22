#ifndef Model_H_
#define Model_H_

#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Point.h"
#include "Triangle.h"

using namespace std;

class Model{
public:
    Model();

    ~Model();

    string get_file_format() const;

    string get_data_format() const;

    vector<string> get_comments() const;

    float get_version_no() const;

    int get_no_vertices() const;

    int get_no_faces() const;

    GLfloat get_spin() const;

    GLfloat get_scale() const;

    pair<GLfloat,GLfloat> get_cursor_pos() const {
        return cursor_pos;
    }
    
    const vector<Point>& get_vertices() const ;

    const vector<Triangle>& get_Triangles() const;

    glm::mat4 get_model() const;

    void set_scale(GLfloat sc);

    void set_spin(GLfloat sp);

    void set_cursor_pos(pair<GLfloat,GLfloat> P);

    void set_model(const glm::mat4& md);

    bool is_inside(Point trans_coord);

    friend ifstream & operator >> (ifstream &fin, Model &model);
private:
    string file_format,data_format;
    GLfloat version_no;
    vector<string> comments;
    int no_vertices,no_faces;
    bool header;
    vector<Point> vertices;
    vector<Triangle> Triangles;
    GLfloat scale,spin;
    pair<GLfloat,GLfloat> cursor_pos;  //last position of the cursor on this model.
    glm::mat4 model;
};

#endif