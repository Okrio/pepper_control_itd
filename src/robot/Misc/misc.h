# include <iostream>
# include <fstream>
#include <itpp/itbase.h>
#include <visp/vpExponentialMap.h>
#include <sstream>
#include <string>
using namespace itpp;
using namespace std;
class misc{
public:
    static void writevec(const std::string &filename, const vec & data);
    static void writevec(const std::string & filename, const Vec<vec> & data);
    static void writemat(const std::string & filename, const mat&data);
    static mat vpMatrixtoMat(const vpMatrix &input);
    static vec vpColVectortoVec(const vpColVector &input);
    static vpColVector vectoVpColVec(const vec &input);
    static std::vector<double> vectoStdVec(const vec &input);
    static vec stdVectoVec(std::vector<double> input);
    static vector<float> vecTovFloat(vec input);
    static double closest( vec to_search, double  find );
    /** read matrix in text file, useful for microphone and source parameters*/
    static mat readMat(const std::string &path);
    static bool  belong(const vec & vlist, const int & val);
    /** read vector in text file, used for the signal*/
    static vec readVec(const string &path);

};
