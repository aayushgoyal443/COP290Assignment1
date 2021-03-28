#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;




int main(int argc, char* argv[]){
    if(argc!=3){
        cout<<"Error: Give two arguments- first as baseline output and second as new method's output.";
        return -1;
    }
    string baselineFile=argv[1];
    string newFile=argv[2];
    ifstream file1 (baselineFile);
    ifstream file2 (baselineFile);
    map<long double, long double> f1,f2;
    long double fr;
    long double den;
    string currentLine;
    while(!file1.eof()){
        file1>>fr;
        file1>>den;
        f1[fr]=den;
    }
    while(!file2.eof()){
        file2>>fr;
        file2>>den;
        f2[fr]=den;
    }
    file1.close();
    file2.close();
    long double sum=0;
    for(auto j:f2){
        sum+=(abs(j.second-f1[j.first])/f1[j.first]);
    }
    sum/=f2.size();
    sum*=100;
    cout<<sum;
}

