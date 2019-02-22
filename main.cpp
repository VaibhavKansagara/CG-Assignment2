#include<bits/stdc++.h>
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Parser.h"
#include "Shader.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"

using namespace std;

const GLfloat screenwidth = 800.0;
const GLfloat screenheight = 600.0;

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow * window = glfwCreateWindow(800,600,"LearnOpengl",NULL,NULL);
    if(window == NULL){
        std::cout<<"Error creating window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //make the current context our window.

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    Shader ourshader("vs.txt","fs.txt");

    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);

    //Create Parser and Model.
    Parser parser("cow.ply");
    Model cube = parser.parse_file();

    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*cube.get_vertices().size(),&cube.get_vertices()[0],GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*cube.get_indices().size(),
                                            &cube.get_indices()[0], GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    ourshader.use();

    //create a view object
    View cube_view;

    //create a controller object.
    Controller cube_controller;
    cube_controller.add(&cube,&cube_view);

    while(!glfwWindowShouldClose(window)){
        cube_controller.process_input(window);
        cube_view.display(cube,ourshader,VAO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //keep in mind that you need to delete VAO before EBO as VAO has a pointer or 
    //something else which connects to EBO.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}