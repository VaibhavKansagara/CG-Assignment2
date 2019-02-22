#ifndef Triangle_H_
#define Triangle_H_

#include "Point.h"
#include "Color.h"

using namespace std;

class Triangle{
public:
    Triangle();

    Triangle (vector<unsigned int> idxs);

    ~Triangle();

    const Color& get_Color() const;

    const vector<unsigned int>& get_indices() const;    

    bool is_inside(Point point,const vector<Point>& Points);

    friend ostream & operator << (ostream &out, const Triangle &c);
private:
    vector<unsigned int> indices;
    Color color;
};

#endif