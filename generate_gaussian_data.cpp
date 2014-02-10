#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>

using namespace std;

int main(int argc, const char* argv[]){

    if(argc % 2 == 0|| argc < 5){
        cout << "Generate n Gaussian deviates given mean/stddev pairs\nUsage:\n" << argv[0] << " n class_label mean_1 stddev_1 .. mean_n stddev_n" << endl;
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
        for(int j = 3; j < argc; j+= 2){
            // Generates random numbers according to the Normal (or Gaussian) random number distribution
            // http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
            normal_distribution<> d(atof(argv[j]),atof(argv[j+1]));
            // Print feature:value pairs
            cout << j/2 << ":" << d(gen) << " ";    
        }
        cout << endl;
    }

    return(0);
}
