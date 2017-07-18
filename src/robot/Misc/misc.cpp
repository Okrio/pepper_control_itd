#include "misc.h"

void misc::writevec(const std::string &filename, const vec & data){

    ofstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        for(int i=0; i<data.length(); i++){
            myfile << data[i] << std::endl;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

}

double misc::closest( vec to_search, double  find ){
    double min_val=1000000;
    double found=0;
    for(int i=0; i<to_search.length();i++){
        if( sqrt(sqr(to_search[i]-find))<min_val){
           min_val=sqrt(sqr(to_search[i]-find));
           found=to_search[i];
        }
    }
    return found;
}

void misc::writevec(const std::string &filename, const Vec<vec> &data) {

    ofstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        for(int i=0; i<data.length(); i++){
            for(int j=0;j<data[i].length();j++)
                myfile << data[i][j] <<" ";
            myfile<<endl;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

}
void misc::writemat(const string &filename, const mat &data) {

    ofstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        for(int i=0; i<data.rows(); i++){
            //myfile << i <<" ";
            for(int j=0;j<data.cols();j++){                
                if (j<data.cols()-1)
                    myfile << data(i,j) <<" ";
                else
                    myfile << data(i,j);
            }
            myfile<<endl;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

}


mat misc::vpMatrixtoMat(const vpMatrix &input){
    mat output=zeros(input.getRows(),input.getCols());
    for(int i=0;i< output.rows();i++)
        for(int j=0; j< output.cols();j++)
            output.set(i,j,input[i][j]);
    return output;
}

bool misc:: belong(const vec & vlist, const int & val){

    int i=0;
    //std::cout<<"vlist:"<<vlist<<std::endl;
    while(i<vlist.length()){
        if (vlist[i]==val){
            //std::cout<<"vlist:"<<vlist<<std::endl;
            return true;
        }
        i++;
    }
    return false;
}

vec misc::vpColVectortoVec(const vpColVector &input){
    vec output=zeros(input.getRows());
    for(int i=0;i< output.length();i++)
        output.set(i,input[i]);
    return output;
}


vpColVector misc::vectoVpColVec(const vec &input){
    vpColVector output;
    output.resize(input.length());
    for(int i=0;i< output.size();i++)
        output[i]=input[i];
    return output;
}

std::vector<double> misc::vectoStdVec(const vec &input){
    std::vector<double> output(input.length(), 0.0);;
    for(int i=0;i< output.size();i++)
        output[i]=input[i];
    //for(int i=0;i< output.size();i++)
        //std::cout << "output:"<<output[i]<< std::endl;
    return output;
}

vec misc::stdVectoVec(std::vector<double>input){

    vec output=zeros(input.size());
    for(int i=0;i< output.length();i++)
        output.set(i,input[i]);
    return output;

}

mat misc::readMat(const std::string &path) {
    std:: string STRING, matrix;
    matrix="";
    mat result;
    std::ifstream infile;
    infile.open (path.c_str());
    if (!infile)
        std::cout << "Unsuccessful!" << std::endl;

    while(infile.good()) // To get you all the lines.
    {
        std::getline(infile,STRING); // Saves the line in STRING.
        //std::cout<<STRING; // Prints our STRING.
        matrix+=STRING;
    }
    infile.close();

    result=matrix;
    return result;

}

vector<float> misc::vecTovFloat(vec input){
    vector<float> output;
    for(int i=0;i<input.length();i++)
        output.push_back(input(i));
    return output;

}


vec misc::readVec(const std::string &path) {
    std:: string STRING, vector;
    vector="";
    vec result;
    std::ifstream infile;
    infile.open (path.c_str());
    if (!infile)
        std::cout << "Unsuccessful!" << std::endl;

    while(infile.good()) // To get you all the lines.
    {
        std::getline(infile,STRING); // Saves the line in STRING.
        //std::cout<<STRING; // Prints our STRING.
        vector+=(STRING+" ");
    }
    infile.close();
    result=vector;
    return result;

}
