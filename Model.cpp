#include "Model.h"
#include <sstream>

using namespace std;

Model::Model(){
    scale = 1.0;
    spin = 0.0;
    model = glm::mat4(1.0f);
}

Model::~Model(){}

string Model::get_file_format() const{
    return file_format;
}

string Model::get_data_format() const{
    return data_format;
}

vector<string> Model::get_comments() const{
    return comments;
}

GLfloat Model::get_version_no() const{
    return version_no;
}

int Model::get_no_vertices() const{
    return no_vertices;
}

int Model::get_no_faces() const{
    return no_faces;
}

GLfloat Model::get_spin() const{
    return spin;
}

GLfloat Model::get_scale() const{
    return scale;
}

vector<GLfloat> Model::get_vertices() const {
    return vertices;
}

vector<unsigned int> Model::get_indices() const{
    return indices;
}

Color Model::get_vertex_color(int idx) const{
    return vertices_color[idx];
}

glm::mat4 Model::get_model() const {
    return model;
}

void Model::set_vertex_color(int idx,const Color& color){
    vertices_color[idx] = color;
}

void Model::set_scale(GLfloat sc){
    scale = sc;
}

void Model::set_spin(GLfloat sp){
    spin = sp;
}

void Model::set_cursor_pos(pair<GLfloat,GLfloat> P){
    cursor_pos = P;
}

void Model::set_model(const glm::mat4& md){
    model = md;
}

bool is_between(float mini,float maxi,float value){
    return (value>=mini && value<=maxi);
}

bool Model::is_inside(Point trans_coord){
    glm::vec4 new_mini = model * glm::vec4(mini.getX(),mini.getY(),mini.getZ(),0.0f);
    glm::vec4 new_maxi = model * glm::vec4(maxi.getX(),maxi.getY(),maxi.getZ(),0.0f);
    if(is_between(new_mini.x,new_maxi.x,trans_coord.getX()) &&
      is_between(new_mini.y,new_maxi.y,trans_coord.getY()) && 
      is_between(new_mini.z,new_maxi.z,trans_coord.getZ())){
        return true;
    }
    else{
        return false;
    }
}

void Model::set_mini(const Point& point){
    mini = point;
}

void Model::set_maxi(const Point& point){
    maxi = point;
}

void Model::compute_adj_list(){
    int no_trg = indices.size()/3;
    adj_list.resize(vertices.size()/3);
    for(int i = 0; i < no_trg; i++){
        adj_list[indices[i*3]].push_back(i);
        adj_list[indices[i*3+1]].push_back(i);
        adj_list[indices[i*3+2]].push_back(i);
    }
}

void Model::compute_vertices_color(){
    int no_trg = indices.size()/3;
    vector<Point> trg_normal;
    for(int i = 0; i < no_trg; i++){
        Point p1 = Point(vertices[indices[i*3+0]],vertices[indices[i*3+0]+1],vertices[indices[i*3+0]+2]);
        Point p2 = Point(vertices[indices[i*3+1]],vertices[indices[i*3+1]+1],vertices[indices[i*3+1]+2]);
        Point p3 = Point(vertices[indices[i*3+2]],vertices[indices[i*3+2]+1],vertices[indices[i*3+2]+2]);
        Point V = p2-p1;
        Point W = p3-p1;
        Point normal;
        normal.setX(V.getY()*W.getZ() - V.getZ()*W.getY());
        normal.setY(V.getZ()*W.getX() - V.getX()*W.getZ());
        normal.setZ(V.getX()*W.getY() - V.getY()*W.getX());
        trg_normal.push_back(normal);
    }
    compute_adj_list();
    for(int i = 0; i < adj_list.size(); i++){
        Point p(0,0,0);
        for(auto e:adj_list[i]){
            p = p + trg_normal[e];
        }
        p.normalize(adj_list[i].size());
        float mag = sqrt( p.getX()*p.getX() + p.getY()*p.getY() + p.getZ()*p.getZ() );
        p.normalize(mag);
        vertices_color.push_back(Color(abs(p.getX()),abs(p.getY()),abs(p.getZ())));
    }   
}

std::ifstream& safeGetline(std::ifstream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::ifstream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

//assumptions:
//vertices will be in float format only.
//no of dimensions = 3.
ifstream & operator >> (ifstream &fin, Model &model){
    string temp;
    fin >> temp >> model.file_format >> model.data_format;
    float minx=1e5,miny=1e5,minz=1e5;
    float maxx=-1e5,maxy=-1e5,maxz=-1e5;
    while(1){
        fin >> temp;
        if(temp != "comment") break;
        string comment;
        safeGetline(fin,comment);
        model.comments.push_back(comment);
    }
    fin >> temp >> model.no_vertices;
    while(1){
        fin >> temp;
        if(temp != "property") break;
        fin >> temp >> temp;
        // do nothing.
    }
    fin >> temp >> model.no_faces;
    while(1){
        safeGetline(fin,temp);
        if(temp == "end_header"){
            break;
        }
    }
    float maxi=0;
    for(int i = 0; i < model.no_vertices; i++){
        float temp1;
        float arr[3];
        for(int j = 0; j < 3; j++){
            fin >> temp1;
            maxi=max(maxi,temp1);
            model.vertices.push_back(temp1);
            if(j==0){
                minx = min(minx,temp1);
                maxx = max(maxx,temp1);
            }
            else if(j==1){
                miny = min(miny,temp1);
                maxy = max(maxy,temp1);
            }
            else{
                minz = min(minz,temp1);
                maxz = max(maxz,temp1);
            }
        }
    }
    for(int i=0;i<model.vertices.size();i++){
        model.vertices[i]/=maxi;
    }
    for(int i = 0; i < model.no_faces; i++){
        unsigned int tmp;
        fin >> tmp;
        unsigned int tmp1;
        for(int j = 0; j < tmp; j++){
            fin >> tmp1;
            model.indices.push_back(tmp1);
        }
    }
    model.set_mini(Point(minx,miny,minz));
    model.set_maxi(Point(maxx,maxy,maxz));

    //compute color of the vertices.
    model.compute_vertices_color();
    return fin;
}
