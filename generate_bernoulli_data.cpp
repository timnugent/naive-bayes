#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <map>

using namespace std;

int main(int argc, const char* argv[]){

    if(argc < 4){
        cout << "Generate n bernoulli deviates given list of probabilities\nUsage:\n" << argv[0] << " n class_label prob_1 .. prob_n" << endl;
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

    int n = atoi(argv[1]);
    int label = atoi(argv[2]);

    for(int i = 0; i < n; i++){
        // Print class labels then feature:value pairs, libsvm format
        if(label > 0){
            cout << "+";
        }
        cout << label << " ";

        for(int j = 3; j < argc; j++){
            // http://en.cppreference.com/w/cpp/numeric/random/bernoulli_distribution
            bernoulli_distribution d(atof(argv[j]));            
            cout << j-2 << ":" << d(gen) << " ";    
        }
        cout << endl;
    }

    return(0);
}
