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

    Shader ourshader("vs.txt","fs.txt");
    Shader lightingShader("light_shader_vs.txt", "light_shaderfs.txt");

    //Create Parser and Model.
    
    Model cube = addModel(name,mode);

    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);
    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*cube.get_vertices(mode).size(),&cube.get_vertices(mode)[0],GL_STATIC_DRAW);
    if(mode == 1){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*cube.get_indices().size(),
                                                &cube.get_indices()[0], GL_STATIC_DRAW);
    }
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    unsigned int ColorBuffer;
    glGenBuffers(1,&ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,ColorBuffer);
    try{
        if(cube.get_vertices(mode).size() != 3*cube.get_vertices_color(mode).size()){
            throw string("Size of vertices and vertices_color is not same");
        }
    }
    catch(const string& s){
        cout << s << endl;
    }
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*cube.get_vertices(mode).size(),&cube.get_vertices_color(mode)[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);



    //light part starts
    Model sphere = addModel("sphere.ply",1);
    unsigned int lightVBO,lightVAO,lightEBO;
    glGenVertexArrays(1,&lightVAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&lightVBO);       //same here.
    glGenBuffers(1,&lightEBO);
    //binds object to context.
    glBindVertexArray(lightVAO);     
    glBindBuffer(GL_ARRAY_BUFFER,lightVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*sphere.get_vertices(1).size(),&sphere.get_vertices(1)[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*sphere.get_indices().size(),
                                            &sphere.get_indices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    //light part ends.



    //create a view object
    View cube_view;
    //create a controller object.
    Controller cube_controller;
    cube_controller.add(&cube,&cube_view);

    while(!glfwWindowShouldClose(window)){
        cube_controller.process_input(window);

        ourshader.use();
        ourshader.setVec3("lightColor", 0.0f, 0.0f, 1.0f);
        ourshader.setVec3("lightPos", lightPos);
        cube_view.display(cube,ourshader,VAO);

        // use lighting shader for source of the light.
        lightingShader.use();
        glm::mat4 model_light = glm::mat4(1.0f);
        model_light = glm::scale(model_light,glm::vec3(0.01,0.01,0.01));
        model_light = glm::translate(model_light, lightPos);
        lightingShader.setMat4("model",model_light);
        int modelLoc = glGetUniformLocation(lightingShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_light));
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, sphere.get_indices().size(), GL_UNSIGNED_INT, 0);

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