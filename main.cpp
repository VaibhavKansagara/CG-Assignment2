#include<bits/stdc++.h>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

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
const GLfloat screenheight = 800.0;

// light souce position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

GLFWwindow * create_window(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    GLFWwindow * window = glfwCreateWindow(800,800,"LearnOpengl",NULL,NULL);
    if(window == NULL){
        std::cout<<"Error creating window";
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window); //make the current context our window.
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    return window;
}

Model addModel(string name,int mode){
    Parser parser(name);
    Model model = parser.parse_file();
    if(mode == 2)
        model.compute_splat_attributes();
    model.set_mode(mode);
    return model;
}

int main(){

    //take user input.
    string name;
    int mode;
    cout << "Enter the name of the model which you want to add:";
    cin>>name;
    cout << "Enter the mode of the model which you want:\n";
    cout << "Options are:1)normal\n2)splat\n";
    cin >> mode;
    //user input ends.

    GLFWwindow * window = create_window();

    //initialise shaders.
    Shader ourshader("vs.txt","fs.txt");
    Shader lightingShader("light_shader_vs.txt", "light_shaderfs.txt");

    //Create Model and add models.
    Model cow = addModel(name,mode);
    cow.pass_info_shader();
    Model beethoven = addModel("big_spider.ply",1);
    beethoven.pass_info_shader();
    //

    glEnable(GL_DEPTH_TEST);

    //light part starts
    Model sphere = addModel("sphere.ply",1);
    sphere.set_light_source(true);
    sphere.pass_info_shader();
    //light part ends.


    //create a view object
    View view;

    //add models to  controller object.
    Controller controller;
    controller.add(&cow);
    controller.add(&beethoven);

    while(!glfwWindowShouldClose(window)){
        controller.process_input(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourshader.use();
        ourshader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourshader.setVec3("lightPos", lightPos);

        //display all the models.
        view.display(cow,ourshader);
        view.display(beethoven,ourshader);

        // use lighting shader and display the source of the light.
        lightingShader.use();
        view.display(sphere,lightingShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //keep in mind that you need to delete VAO before EBO as VAO has a pointer or 
    //something else which connects to EBO.
    glfwTerminate();
    return 0;
}