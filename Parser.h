#ifndef Parser_H_
#define Parser_H_

#include "Model.h"

using namespace std;

class Parser{
public:
    Parser();

    ~Parser();

    Parser(string name);

    string get_name() const;

    void set_name(string name);

    Model parse_file();
private:
    string filename;
};

#endif