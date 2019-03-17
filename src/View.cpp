#include<bits/stdc++.h>

#include "../include/View.h"

View::View(){}

View::~View(){}

glm::mat4 View::get_view() const {
    return view;
}

glm::mat4 View::get_projection() const {
    return projection;
}

void View::set_view(const glm::mat4& vw) {
    view = vw;
}

void View::set_projection(const glm::mat4& pr){
    projection = pr;
}

void View::display_lightsource(const Model& model,const Shader& ourshader){
    glm::mat4 model_light = glm::mat4(1.0f);
    model_light = glm::scale(model_light,glm::vec3(0.01,0.01,0.01));
    model_light = glm::translate(model_light, lightPos);
    ourshader.setMat4("model",model_light);
    int modelLoc = glGetUniformLocation(ourshader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_light));
    glBindVertexArray(model.get_VAO());
    glDrawElements(GL_TRIANGLES, model.get_indices().size(), GL_UNSIGNED_INT, 0);
}

void View::display(const Model& model,const Shader& ourshader) {
    if(model.get_light_source()){
        display_lightsource(model,ourshader);
        return;
    }
    set_view(glm::mat4(1.0f));
    set_projection(glm::mat4(1.0f));

    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(scale,glm::vec3(model.get_scale(),model.get_scale(),model.get_scale()));
    glm::mat4 model_matrix = model.get_translate()*model.get_rotate()*scale;

    int modelLoc = glGetUniformLocation(ourshader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    int viewLoc = glGetUniformLocation(ourshader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(get_view()));

    int projectLoc = glGetUniformLocation(ourshader.ID, "projection");
    glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(get_projection()));

    glBindVertexArray(model.get_VAO());
    if(model.get_mode() == 1){
        glDrawElements(GL_TRIANGLES, model.get_indices().size(), GL_UNSIGNED_INT, 0);
    }
    else if(model.get_mode() == 2){
        glDrawArrays(GL_TRIANGLES,0,(model.get_vertices(2).size()/3));
    }
    glBindVertexArray(0);
}
