// Naive Bayes Implementation
// (c) Tim Nugent 2014
// timnugent@gmail.com

#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

vector<string> &split(const string &s, char delim, vector<std::string> &elems) {
    stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void usage(const char* prog){

   cout << "Read training data then classify test data using naive Bayes:\nUsage:\n" << prog << " [options] training_data test_data" << endl << endl;
   cout << "Options:" << endl;
   cout << "-d <int> Decsion rule. 1 = gaussian (default)" << endl;
   cout << "                       2 = multinomial" << endl;
   cout << "                       3 = bernoulli" << endl;
   cout << "-a       Smoothing parameter alpha. default 1.0 (Laplace)" << endl;      
   cout << "-v       Verbose." << endl << endl;      
}

int main(int argc, const char* argv[]){

    // Smoothing factor
    double alpha = 1.0;
    // Decision rule
    int decision = 1;
    // Verbose
    int verbose = 0;

    if(argc < 3){
        usage(argv[0]);
        return(1);
    }else{
        cout << "# called with: ";
        for(int i = 0; i < argc; i++){
            cout << argv[i] << " ";
            if(string(argv[i]) == "-d" && i < argc-1){
                decision = atoi(argv[i+1]);
            }
            if(string(argv[i]) == "-a" && i < argc-1){
                alpha = atof(argv[i+1]);
            }
            if(string(argv[i]) == "-v"){
                verbose = 1;
            }
            if(string(argv[i]) == "-h"){
                usage(argv[0]);
                return(1);
            }
        }
        cout << endl;
    }
    switch(decision){
        case 2:
            cout << "# decision rule: multinomial" << endl;
            break;
        case 3:
            cout << "# decision rule: bernoulli" << endl;
            break;   
        default:
            cout << "# decision rule: gaussian" << endl;
            break;
    }    
    cout << "# alpha param:   " << alpha << endl;
    cout << "# training data: " << argv[argc-2] << endl;
    cout << "# test data:     " << argv[argc-1] << endl;

    unsigned int n_total = 0;
    map<int,vector<vector<double> > > data;
    map<int,int> n;
    map<int,double> priors;
    map<int,vector<double> > multinomial_likelihoods;
    map<int,int> multinomial_sums;
    map<int,vector<double> > sum_x;
    map<int,vector<double> > means;
    map<int,vector<double> > variances;

    ifstream fin(argv[argc-2]);
    fin.ignore();
    string line;
    while (getline(fin, line)){
        if(line.length()){
            if(line[0] != '#' && line[0] != ' '){
                vector<string> tokens = split(line,' ');
                vector<double> values;
                int label = atoi(tokens[0].c_str());
                for(unsigned int i = 1; i < tokens.size(); i++){
                    vector<string> feat_val = split(tokens[i],':');
                    values.push_back(atof(feat_val[1].c_str()));   
                    if(sum_x.find(label) == sum_x.end()){
                        vector<double> empty;
                        for(unsigned int j = 1; j < tokens.size(); j++){
                            empty.push_back(0.0);
                        }    
                        sum_x[label] = empty;
                    }
                    sum_x[label][i-1] += values[i-1];
                    multinomial_sums[label] += values[i-1];
                }
                if(values.size() != sum_x[label].size()){
                    cout << "# inconsistent feature count! sparse data not supported yet." << endl;
                    //cout << "# " << values.size() << " vs " << sum_x[label].size() << endl;
                    //cout << line << endl;
                    fin.close();
                    exit(1);
                }
                data[label].push_back(values);
                n[label]++;
                n_total++;
            }    
        }
    }
    fin.close();

    for(auto it = sum_x.begin(); it != sum_x.end(); it++){

        priors[it->first] = (double)n[it->first]/n_total;

        if(it->first > 0){
            printf ("class +%i prior: %1.3f\n",it->first,priors[it->first]);
        }else{
            printf ("class %i prior: %1.3f\n",it->first,priors[it->first]);
        }
        cout << "feature\tmean\tvar\tstddev\tmnl" << endl;

        // Calculate means
        vector<double> feature_means;
        for(unsigned int i = 0; i < it->second.size(); i++){
            feature_means.push_back(sum_x[it->first][i]/n[it->first]);
        }        
        
        // Calculate variances
        vector<double> feature_variances(feature_means.size());
        for(unsigned int i = 0; i < data[it->first].size(); i++){
            for(unsigned int j = 0; j < data[it->first][i].size(); j++){
                feature_variances[j] += (data[it->first][i][j]-feature_means[j])*(data[it->first][i][j]-feature_means[j]);
            }    
        }
        for(unsigned int i = 0; i < feature_variances.size(); i++){
            feature_variances[i] /= data[it->first].size();
        }

        // Calculate multinomial likelihoods
        for(unsigned int i = 0; i < feature_means.size(); i++){
            double mnl = (sum_x[it->first][i]+alpha)/(multinomial_sums[it->first]+(alpha*feature_means.size()));
            //cout << sum_x[it->first][i] << " + 1 / " << multinomial_sums[it->first] << " + " << feature_means.size() << endl;
            multinomial_likelihoods[it->first].push_back(mnl);
        }

        for(unsigned int i = 0; i < feature_means.size(); i++){            
            printf("%i\t%2.3f\t%2.3f\t%2.3f\t%2.3f\n",i+1,feature_means[i],feature_variances[i],sqrt(feature_variances[i]),multinomial_likelihoods[it->first][i]);
            //cout << feature_means[i] << "\t" << sqrt(feature_variances[i]) << endl;
        }
        means[it->first] = feature_means;
        variances[it->first] = feature_variances;        

    }    

    // Classify
    cout << "Classifying:" << endl;
    if(verbose) cout << "class\tprob\tresult" << endl;
    int correct = 0;
    int total = 0;

    fin.open(argv[argc-1]);
    while (getline(fin, line)){
        if(line[0] != '#' && line[0] != ' ' && line[0] != '\n'){
            vector<string> tokens = split(line,' ');
            vector<double> values;
            int label = atoi(tokens[0].c_str());
            for(unsigned int i = 1; i < tokens.size(); i++){
                vector<string> feat_val = split(tokens[i],':');
                values.push_back(atof(feat_val[1].c_str()));      
            }

            int predlabel = 0;
            double maxlikelihood = 0.0;
            double denom = 0.0;
            vector<double> probs;
            for(auto it = priors.begin(); it != priors.end(); it++){
                double numer = priors[it->first];
                for(unsigned int j = 0; j < values.size(); j++){
                    switch(decision){
                        case 2:
                            // Multinomial      
                            if(values[j]){
                                numer *= pow(multinomial_likelihoods[it->first][j],values[j]); 
                            }
                            break;
                        case 3:
                            // Bernoulli
                            numer *= (pow(means[it->first][j],values[j]) * pow((1.0-means[it->first][j]),(1.0-values[j])));
                            break;   
                        default:
                            // Gaussian
                            numer *= (1/sqrt(2*M_PI*variances[it->first][j])*exp((-1*(values[j]-means[it->first][j])*(values[j]-means[it->first][j]))/(2*variances[it->first][j])));
                            break;                         
                    }
                }
                /*
                if(verbose){
                    if(it->first > 0){
                        cout << "+" << it->first << ":" << numer << endl;
                    }else{
                        cout << it->first << ":" << numer << endl;
                    }
                }
                */
                if(numer > maxlikelihood){
                    maxlikelihood = numer;
                    predlabel = it->first;
                }   
                denom += numer;
                probs.push_back(numer);
            }    
            //for(unsigned int j = 0; j < probs.size(); j++){
            //    cout << probs[j]/denom << " ";
            //}

            if(verbose){
                if(predlabel > 0){
                    printf ("+%i\t%1.3f\t", predlabel,(maxlikelihood/denom)); 
                }else{
                    printf ("%i\t%1.3f\t", predlabel,(maxlikelihood/denom));
                }
            }
            if(label){
                if(predlabel == label){
                    if(verbose) cout << "correct" << endl;
                    correct++;
                }else{
                    if(verbose) cout << "incorrect" << endl;
                }
            }else{
                if(verbose) cout << "<no label>" << endl;
            }
            total++;
        }    
    }
    fin.close();
    printf ("Accuracy: %3.2f %% (%i/%i)\n", (100*(double)correct/total),correct,total);

    return(0);

}
