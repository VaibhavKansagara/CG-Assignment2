#include "Triangle.h"

using namespace std;

Triangle::Triangle(){}

Triangle::Triangle(vector<unsigned int> idxs):indices(idxs){}

Triangle::~Triangle(){}

const Color& Triangle::get_Color () const {
    return color;
}
 
const vector<unsigned int>& Triangle::get_indices() const{
	return indices;
}

bool Triangle::is_inside(Point point,const vector<Point>& Points){
	float OA = point.get_distance(Points[indices[0]]);
	float OB = point.get_distance(Points[indices[1]]);
	float OC = point.get_distance(Points[indices[2]]);

	float AB = Points[indices[0]].get_distance(Points[indices[1]]);	//distance(AB)
	float BC = Points[indices[1]].get_distance(Points[indices[2]]);	//distance(BC)
	float AC = Points[indices[0]].get_distance(Points[indices[2]]);	//distance(AC)

	return (OA+OB+OC) <= (AB+BC+AC);
}

ostream & operator << (ostream &out, const Triangle &c){
    return out;
}
