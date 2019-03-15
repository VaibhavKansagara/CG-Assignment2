#ifndef View_H_
#define View_H_

#include "Shader.h"
#include "Model.h"
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

extern glm::vec3 lightPos;
extern const GLfloat screenwidth,screenheight;
extern const unsigned int VAO;

class View{
public:
    View();

    ~View();

    glm::mat4 get_view() const;

    glm::mat4 get_projection() const;

    void set_view(const glm::mat4& vw);

    void set_projection(const glm::mat4& pr);

    void display(const Model& model,const Shader& ourshader);

    void display_lightsource(const Model& model,const Shader& ourshader);
private:
	glm::mat4 view,projection;
};

#endif