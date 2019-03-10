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
#include "Color.h"
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

    Color get_vertex_color(int idx) const; 

    glm::vec3 get_cursor_pos() const;
    
    vector<GLfloat> get_vertices() const ;

    vector<Color> get_vertices_color() const;

    vector<unsigned int> get_indices() const;

    glm::mat4 get_translate() const;

    glm::mat4 get_rotate() const;

    GLfloat get_scale() const;

    void set_vertex_color(int idx,const Color& color);

    void set_translate(const glm::mat4& tr);

    void set_scale(GLfloat sc);

    void set_rotate(const glm::mat4& rr);

    void set_cursor_pos(glm::vec3 pos);

    void set_mini(const Point& point);

    void set_maxi(const Point& point);

    bool is_inside(Point trans_coord);

    void compute_adj_list();

    void compute_vertices_color();

    friend ifstream & operator >> (ifstream &fin, Model &model);
private:
    string file_format,data_format;
    GLfloat version_no;
    vector<string> comments;
    int no_vertices,no_faces;
    bool header;
    vector<GLfloat> vertices;
    vector< vector<int> > adj_list;
    vector<unsigned int> indices;
    vector<Color> vertices_color;
    glm::vec3 currpos;
    GLfloat scale;
    glm::mat4 translate,rotate;
    Point mini,maxi;
};

#endif