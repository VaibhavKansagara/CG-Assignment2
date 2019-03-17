#include "../include/Controller.h"
#define NOT_ANY_MODEL -1

using namespace std;

Controller::Controller(){}

Controller::Controller(vector<Model*> mv):model_vector(mv){}

void Controller::add(Model* model){
    model_vector.push_back(model);
}

//because we have same view and projection matrix for all models. 
Point Controller::get_trans_coord(double x,double y){
    float winX,winY,winZ;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	winX = (winX / 400) - 1;
	winY = (winY / 400) - 1;
	winZ = winZ * 2  - 1;
	return Point(winX,winY,winZ);
}

int Controller::find(double x,double y){
    Point trans_coord = get_trans_coord(x,y);
    int len = model_vector.size();
    for(int i=0;i<len;i++){
        glm::vec3 pos = glm::vec3(trans_coord.getX(),trans_coord.getY(),trans_coord.getZ());
        pos = glm::inverse(model_vector[i]->get_model()) * glm::vec4(pos,1.0f);
        Point temp(pos.x,pos.y,pos.z);
        if(model_vector[i]->is_inside(temp) == true){
            if(model_vector[i]->is_select() == false and 
                  model_vector[i]->get_is_select_rotate() == false){
                model_vector[i]->set_cursor_pos(glm::vec3(trans_coord.getX(),
                                                trans_coord.getY(),trans_coord.getZ()));
                model_vector[i]->set_selected(true);
            }
            return i;
        }
    }
    return -1;
}

void Controller::process_input(GLFWwindow* window){
    double x,y;
    glfwGetCursorPos(window,&x,&y);
    int active_model = find(x,y);
    // cout << "active-model:" << active_model <<endl;
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }

    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_UP) == GLFW_REPEAT){
    	lightPos.y += 0.5;
    }

    if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_REPEAT){
    	lightPos.y -= 0.5;
    }

    if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_REPEAT){
    	lightPos.x -= 0.5;
    }

    if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_REPEAT){
    	lightPos.x += 0.5;
    }

    if(active_model == NOT_ANY_MODEL) return;

    if(glfwGetKey(window,GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_KP_ADD) == GLFW_REPEAT){
        Model* model = model_vector[active_model];
        model->set_scale(model->get_scale() + 0.05);
    }

    if(glfwGetKey(window,GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_KP_SUBTRACT) == GLFW_REPEAT){
    	Model* model = model_vector[active_model];
        model->set_scale(model->get_scale() - 0.05);
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
            glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_REPEAT){
        float winX, winY,winZ;
        winX = x;
        winY = 800 - y;
        winX = (winX / 400) - 1;
        winY = (winY / 400) - 1;
        glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        winZ = winZ*2-1;

        Model* model = model_vector[active_model];
        glm::vec3 oldPos = model->get_cursor_pos();
        glm::vec3 newPos = glm::vec3(winX,winY,winZ);
        glm::vec3 diff = newPos - oldPos;
        glm::mat4 translate = glm::translate(model->get_translate(),glm::vec3(diff.x,diff.y,0.0f));
        model->set_translate(translate);
        model->set_cursor_pos(newPos);
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        Model* model = model_vector[active_model];
        model->set_selected(false);
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS ||
            glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_REPEAT){
        Model* model = model_vector[active_model];
        model->set_select_rotate(true);

        float winX, winY,winZ;
        winX = x;
        winY = 800 - y;
        winX = (winX / 400) - 1;
        winY = (winY / 400) - 1;
        glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
        winZ = winZ*2-1;

        glm::vec3 newpos = glm::vec3(winX,winY,winZ);
        glm::vec3 oldPos = model->get_cursor_pos();
        float d = sqrt(newpos.x * newpos.x + newpos.y * newpos.y);
        glm::vec3 p1;
        p1.x = newpos.x;
        p1.y = newpos.y;
        p1.z = cos((3.14159f / 2.0f) * (d < 1.0 ? d : 1.0f));

        d = sqrt(oldPos.x * oldPos.x + oldPos.y * oldPos.y);
        glm::vec3 p2;
        p2.x = oldPos.x;
        p2.y = oldPos.y;
        p2.z = cos((3.14159f / 2.0f) * (d < 1.0 ? d : 1.0f));

        float angle = 90.f * sqrt((p2.x - p1.x) * (p2.x - p1.x) +
                                (p2.y - p1.y) * (p2.y - p1.y) +
                                (p2.z - p1.z) * (p2.z - p1.z));
        glm::vec3 normal = glm::cross(p1, p2);
        if (normal.x == 0 and normal.y == 0 and normal.z == 0) {
        return;
        }
        glm::quat quat = glm::angleAxis(glm::radians(angle), glm::normalize(normal));
        model->set_rotate(glm::mat4_cast(quat) * model->get_rotate());
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        Model* model = model_vector[active_model];
        model->set_select_rotate(false);
    }
}

Controller::~Controller(){}
