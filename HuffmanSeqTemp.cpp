#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <queue>
#include <fstream>
#include <thread>
#include "utimer.hpp"
#include "BuildHuffman.hpp"


using namespace std;
int len; // len of the string
string myString; // string tht contain the string to process




// function that transform the original string in the encoded string 
string Encode(unordered_map<char,string>Huffcode)
{
    string result;
    for(int i=0;i<len;i++)
    { 
        result+=Huffcode[myString[i]];
    }
    return result;
}

//compute frequency of the letter in the text
void ComputeFrequency(unordered_map<char,int> &mp)
{
    for(int i=0;i<len;i++)
    {         
        mp[myString[i]]++;
    }
    
}

int main(int argc, char* argv[])
{
    
    string Filename;
    string result;
    string line;
    ofstream out("textOut.bin",ios::out | ios::binary);
    unordered_map<char,int> mpp; // A map for each character with its frequency
    unordered_map<char,string>Huffcode;// map for each character the bit string 
    long usecRead;

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return(0);
    }
    if(argc==1)
    {
        cout << "Usage is: " << argv[0] << " fileName" << endl; 
        return 0;
    }
    Filename=argv[1]; // take the name of the file
    ifstream inputFile(Filename);// open the inputfilestream
    if(inputFile.good()==false)
    {
        cout << "The file: " << argv[1] << " does not exists" << endl;  
        return 0;
    }
    // take the input string
    {
        utimer t0("parallel computation",&usecRead);
        while (getline(inputFile, line))
        {
            myString += line;
        }
    }
   
    len=myString.size();
 
    long  freq; 
    long buildtemp;
    long encode;
    {
        utimer t0("parallel computation",&freq); 
        ComputeFrequency(ref(mpp));
    }
    
    {
        utimer t0("parallel computation",&buildtemp);  
        nodeTree* Root=BuildHuffman(mpp);
        saveEncode(Root,"",Huffcode);
    }   
    {
        utimer t0("parallel computation",&encode);
        result=Encode(Huffcode);
        
    }
    /*cout << "End spent for Frequency " << freq << " usecs" << endl;
    cout << "End spent for build and traverse " << buildtemp << " usecs" << endl;
    cout << "End spent  encode " << encode << " usecs" << endl;*/

    cout << "r," <<  usecRead << endl;
    cout << "F," << freq  << endl;
    cout << "b," << buildtemp <<  endl;
    cout << "e," << encode  << endl;

   



   
    long usecWrite;
    {
        utimer t0("parallel computation",&usecWrite);
        WriteFile(result);
    }
    cout << "w," << usecWrite  << endl;
   // cout << "End spent for Write the encoded file " << usecWrite << " usecs" << endl;
       
}