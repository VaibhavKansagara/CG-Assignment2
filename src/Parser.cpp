#include "../include/Parser.h"

Parser::Parser(){}

Parser::Parser(string name):filename(name){}

Parser::~Parser(){}

string Parser::get_name() const {
    return filename;
}

void Parser::set_name(string name){
    filename = name;
}

Model Parser::parse_file(){
    Model model;
    string file_format;
    ifstream fin;
    fin.open(filename);
    if(!fin){
        cerr<<"Error in opening the file\n";
    }

    fin>>file_format;
    if(file_format != "ply"){
        cerr << "File format not supported\n";
    }
    //take input 
    fin >> model;

    fin.close();

    return model;
}
