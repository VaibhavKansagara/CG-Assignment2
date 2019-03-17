#ifndef Controller_H_
#define Controller_H_

#include "View.h"
#include "Model.h"

using namespace std;

extern const GLfloat screenwidth,screenheight;
extern glm::vec3 lightPos;

class Controller{
public:
    Controller();
    
    Controller(vector<Model*> mv);

    ~Controller();

    void add(Model* model);

    Point get_trans_coord(double x,double y);

    int find(double x,double y);

    void process_input(GLFWwindow* window);

private:
	vector<Model*> model_vector;
	View* view;
};

#endif