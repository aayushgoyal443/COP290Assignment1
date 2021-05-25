#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;




int main(int argc, char* argv[]){
    if(argc!=4){
        cout<<"Error: Give three arguments- first as baseline output, second as new method's output and third as no. of frames being skipped";
        return -1;
    }
    int skip=stoi(argv[3]);
    skip+=1;
    string baselineFile=argv[1];
    string newFile=argv[2];
    ifstream file1 (baselineFile);
    ifstream file2 (newFile);
    long double fr;
    long double den;
    string currentLine;
    string label;
    if(!file1.eof())file1>>label;
    if(!file1.eof())file1>>label;
    if(!file2.eof())file2>>label;
    if(!file2.eof())file2>>label;
    int count =1;
    int frCount=0;
    long double tSum=0;
    long double sum=0;
    while(!file1.eof() && !file2.eof()){
        file1>>fr;
        file1>>den;
        tSum+=den;
        if(count%skip==0){
            tSum/=skip;
            file2>>fr;
            file2>>den;
            sum+=(abs(tSum-den)/(tSum+0.00000001));
            cout<<frCount<<" "<<tSum<<" "<<den<<endl;
            tSum=0;
            frCount++;
        }
        count++;
    }
    while(!file1.eof()){
        file1>>fr;
        file1>>den;
    }
    while(!file2.eof()){
        file2>>fr;
        file2>>den;
    }
    file1.close();
    file2.close();
    sum/=frCount;
    sum*=100;
    cout<<"Percent error: "<<sum<<endl;
}

