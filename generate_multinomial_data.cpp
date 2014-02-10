#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <map>

using namespace std;

int main(int argc, const char* argv[]){

    if(argc < 4){
        cout << "Generate n multinomial deviates given list of integer weights\nUsage:\n" << argv[0] << " n class_label weight_1 .. weight_n" << endl;
        return(1);
    }else{
        cout << "# called with: ";
        for(int i = 0; i < argc; i++){
            cout << argv[i] << " ";
        }
        cout << endl;
    }
    
    // Random number engine based on Mersenne Twister algorithm
    // http://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
    random_device rd;
    mt19937 gen(rd());
    vector<int> weights;
    for(int i = 3; i < argc; i++){
        weights.push_back(atoi(argv[i]));
    }
    // http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
    discrete_distribution<> d(weights.begin(),weights.end());
    map<int, int> m;
    int n = atoi(argv[1]);
    int label = atoi(argv[2]);
    for(int i = 0; i < n; i++){
        // Print class labels then feature:value pairs, libsvm format
        if(label > 0){
            cout << "+";
        }
        cout << label << " ";

        m.clear();
        // 200 = sample size
        for(int j = 0; j < 200; j++){
            m[d(gen)]++;
        }
        for(int j = 3; j < argc; j++){
            cout << j-2 << ":" << m[j-3] << " ";    
        }
        cout << endl;
    }    

    return(0);
}
