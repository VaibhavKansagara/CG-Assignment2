#include "Controller.h"
#define NOT_ANY_MODEL -1

using namespace std;

Controller::Controller(){}

Controller::Controller(vector<Model*> mv,vector<View*> vv):model_vector(mv),view_vector(vv){}

void Controller::add(Model* model,View* view){
    model_vector.push_back(model);
    view_vector.push_back(view);
}

//because we have same view and projection matrix for all models. 
Point Controller::get_trans_coord(double x,double y){
    glm::mat4 view = view_vector[0]->get_view();
    glm::mat4 proj = view_vector[0]->get_projection();
    glm::mat4 pv_inverse = glm::matrixCompMult(glm::inverse(view),glm::inverse(proj));
    glm::vec4 v(x,y,0,1);
    glm::vec4 ans = pv_inverse * v;
    return Point(ans.x,ans.y,ans.z); 
}

int Controller::find(double x,double y){
    Point trans_coord = get_trans_coord(x,y);
    int len = model_vector.size();
    for(int i=0;i<len;i++){
        if(model_vector[i]->is_inside(trans_coord) == true){
            return i;
        }
    }
    return -1;
}

void Controller::process_input(GLFWwindow* window){
    double x,y;
    glfwGetCursorPos(window,&x,&y);
    int active_model = find(x,y);
    bool left_press = false;
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }

    if(active_model == NOT_ANY_MODEL) return;

    if(glfwGetKey(window,GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_KP_ADD) == GLFW_REPEAT){
        Model* model = model_vector[active_model];
    	model->set_scale(model_vector[active_model]->get_scale() + 0.05);
        model->set_model(glm::mat4(1.0f));
        model->set_model(glm::scale(model->get_model(),
                glm::vec3(model->get_scale(), model->get_scale(), model->get_scale()))); 
        model_vector[active_model]->set_model(glm::translate(model->get_model(),
                glm::vec3(model->get_cursor_pos().first,model->get_cursor_pos().second,0.0f)));
    }

    if(glfwGetKey(window,GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_KP_SUBTRACT) == GLFW_REPEAT){
    	Model* model = model_vector[active_model];
        model->set_scale(model_vector[active_model]->get_scale() - 0.05);
        model->set_model(glm::mat4(1.0f));
        model->set_model(glm::scale(model->get_model(),
                glm::vec3(model->get_scale(), model->get_scale(), model->get_scale()))); 
        model_vector[active_model]->set_model(glm::translate(model->get_model(),
                glm::vec3(model->get_cursor_pos().first,model->get_cursor_pos().second,0.0f)));
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        left_press = true;
    }

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        left_press = false;
    }

    if(left_press == true){
        double temp = sqrt(screenwidth*screenwidth + screenheight*screenheight);
        model_vector[active_model]->set_cursor_pos({x/temp,y/temp});
    }
}

Controller::~Controller(){}
