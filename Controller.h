#ifndef Controller_H_
#define Controller_H_

#include "View.h"

using namespace std;

extern const GLfloat screenwidth,screenheight;

class Controller{
public:
    Controller();
    
    Controller(vector<Model*> mv,vector<View*> vv);

    ~Controller();

    void add(Model* model,View* view);

    Point get_trans_coord(double x,double y);

    int find(double x,double y);

    void process_input(GLFWwindow* window);

private:
	vector<Model*> model_vector;
	vector<View*> view_vector;
};

#endif