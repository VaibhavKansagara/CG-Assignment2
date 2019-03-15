#include "Controller.h"
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
	winY = (winY / 400) - 1 + 0.225;
	winZ = winZ * 2  - 1;
	return Point(winX,winY,winZ);

}

int Controller::find(double x,double y){
    Point trans_coord = get_trans_coord(x,y);
    int len = model_vector.size();
    for(int i=0;i<len;i++){
        if(model_vector[i]->is_inside(trans_coord) == true){
            double tempX = x/400-1;
            double tempY = 800 -y;
            tempY = tempY/400-1;
            if(model_vector[i]->is_select() == false){
                model_vector[i]->set_cursor_pos(glm::vec3(tempX,tempY,0.0));
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
        // cout<< "oldPos:"<<oldPos.x << " " << oldPos.y<<endl;
        // cout<< "newPos:"<<newPos.x << " " << newPos.y<<endl;
        // cout << diff.x << " " << diff.y<<endl;
        glm::mat4 translate = glm::translate(model->get_translate(),glm::vec3(diff.x,diff.y,0.0f));
        model->set_translate(translate);
        model->set_cursor_pos(newPos);
        // oldPos = model->get_cursor_pos();
        // cout<< oldPos.x << " " << oldPos.y<<endl;
        // cout<< newPos.x << " " << newPos.y<<endl;
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        Model* model = model_vector[active_model];
        model->set_selected(false);
    }
}

Controller::~Controller(){}
