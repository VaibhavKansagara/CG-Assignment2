#include<bits/stdc++.h>

#include "View.h"

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

void View::display(const Model& model,const Shader& ourshader,const unsigned int VAO) {
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    set_view(glm::mat4(1.0f));
    set_projection(glm::mat4(1.0f));

    int modelLoc = glGetUniformLocation(ourshader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model.get_model()));

    int viewLoc = glGetUniformLocation(ourshader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(get_view()));

    int projectLoc = glGetUniformLocation(ourshader.ID, "projection");
    glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(get_projection()));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, model.get_indices().size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_LINES, 18, GL_UNSIGNED_INT, 0);
}