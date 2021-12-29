#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

/* Author : Merve horuz */

using namespace std;

class releation{
    public:
        /*default constructor*/
        releation(){ };
        /*it reads the file for each set*/
        void readFileOneSet();
        /*this function checks that whether this set has transitivity property*/
        void checkTransitivity();
        /*this function checks that whether this set has symmetry property*/
        void checkSymmetry();
        /*this function checks that whether this set has antisymmetry property*/
        void checkAntiSymmetry();
        /*this function checks that whether this set has reflexivity property*/
        void checkReflexivity();
        /*it writes the information to file for each set*/
        void writeFile(int flag);
        inline string getFileElement() {return fileElemnt; };
        inline int getOrder() { return order; };
        inline void setOrder(int orderVal) { order = orderVal; };
        /*it takes releations in each set*/
        void eachSet();
        /*helper function*/
        string makeSet(char first, char second, string found);
    
    private:
        /*the elements of the set A*/
        vector<char> elements;
        /*each relation in R*/
        vector<vector<char> > releationR;
        /*strings that are written to file output.txt*/
        string fileElemnt;
        /*into readFile is read the file input.txt*/
        string readFile;
        /*to take order for each set for ex: 0,1,2...*/
        int order;
};

int main(){

    releation obje;

    obje.readFileOneSet();

    /*due to there is eight set, it iterates eight times*/
    for(int i=0;i<8;i++){
        
        obje.setOrder(i);
        obje.eachSet();
        obje.checkReflexivity();
        obje.checkSymmetry();
        obje.checkAntiSymmetry();
        obje.checkTransitivity();
        obje.writeFile(i);
    }
}

void releation :: readFileOneSet(){

    ifstream inStream;
    inStream.open("input.txt");
    string str;
    int j=0;

    if(inStream.fail())
        cerr << "error opening the file : input.txt";

    else{
        /*in this block, the elements of one set are putted into elements vector*/
        for(int i=0;!inStream.eof();i++){
            getline(inStream,str);      /*taking one line from file*/
            
            /*putting into string the each line*/
            for(int k=0;str[k] != '\0';k++,j++){
                readFile.resize(j+1);
                readFile[j] = str[k];
            }
            /*putting to end of each line the newline character*/
            readFile.resize(j+1);
            readFile[j] = '\n';
            j++;
                   
        }
        /*because of there is 2 newline character in input.txt*/
        readFile[j-2] = '\0';
    }
    inStream.close();
}

void releation :: eachSet(){

    string str;
    int temp = 0,j,flag;
    vector <char> temporary;

    /*in this block, beginning of new set is found*/
    for(j=0;readFile[j] != '\0';j++){
        if(readFile[j] == '\n')   j++;
        for(int i=0;readFile[j] != '\n';i++,j++){
            str.resize(i+1);
            str[i] = readFile[j];
        }
        /*for ex : if str == 9, then temp is incremented by one*/
        if(str[1] != ',')
            temp++;
        /*index number is obtained according to the set number*/
        if(temp-1 == getOrder() && str[1] != ',')
            flag = j;
    }

    /*set elements are obtained. (a,b,c,d,e)*/
    for(j=flag+1;readFile[j] != '\n';j++)
        if(readFile[j] != ',')
            elements.push_back(readFile[j]);   
    
    flag = ++j; /*skip newline character*/
   
    /*in this block, it reads the each releation in R*/
    for(int i=0;str[1] == ',' && i<readFile.length();i++){
        str.clear();
        if(readFile[flag] == '\n')    flag++;

        /*taking each releation row by row*/
        for(j=0;readFile[flag] != '\n';flag++,j++){
            str.resize(j+1);
            str[j] = readFile[flag];
        }
        if(str[1] == ','){
            temporary.push_back((str[0])); /*first element*/
            temporary.push_back((str[2])); /*second element*/
            releationR.push_back(temporary);
            temporary.clear();   /*cleaning previous data because same data assigns to releationR vector*/
        }
        /*if it encounters the indicator (9,8,7,6...)*/
        else{
            temporary.push_back('.'); /*first element*/
            temporary.push_back('.'); /*second element*/
            releationR.push_back(temporary);
            temporary.clear();   /*cleaning previous data because same data assigns to releationR vector*/
        }
    }

    /*obtaining that notation -> (a,b) and putting into fileElemnt string the each releation*/
    for(int i=0,k=0;releationR[k][0] != '.';i += 5,k++){
        fileElemnt.resize(i+7);
        fileElemnt[i+2] = '(';
        fileElemnt[i+3] = (releationR[k][0]);
        fileElemnt[i+4] = ',';
        fileElemnt[i+5] = (releationR[k][1]);
        fileElemnt[i+6] = ')';
    }
    fileElemnt[0] = 'n'; /*for indicator n*/
    fileElemnt[1] = '\n';
}

void releation ::  checkTransitivity(){

    int flag = 0,result = 0, allTrans = 0,j,flag2=0,flag3 = 0;
    string str,found,found2,found3,notFound,notFound2,notFound3;
    char newR[2];

    for(int i=0;releationR[i][0] != '.';i++){
        for(int k=0;releationR[k][0] != '.';k++){
            flag2 = 0;
            flag = 0;
            /*if ,(a,b) and (b,c), second charachter of first releation and first character of second releation is same*/
            if(releationR[i][1] == releationR[k][0]){
                
                allTrans++;    /*number of transitive elements*/ 
                flag = 1;
                newR[0] = releationR[i][0];     /*for (a,b) and (b,c), new[0] = a*/
                newR[1] = releationR[k][1];     /*for (a,b) and (b,c), new[1] = c*/

                /*finding (a,c) in set*/
                for(j=0;releationR[j][0] != '.';j++){
                    if(releationR[j][0] == newR[0] && releationR[j][1] == newR[1]){
                        result++;
                        flag2=1;
                    }
                }
                /*if it is not transitive, then i stored the elemnts that is not transitive*/
                if(!flag2){
                    notFound = makeSet(newR[0], newR[1], found);
                    notFound2 = makeSet(releationR[i][0],releationR[i][1],found2);
                    notFound3 = makeSet(releationR[k][0],releationR[k][1],found3);
                }
                /*if it is transitive, then i stored the elemnts that is transitive*/
                else if(flag2 && releationR[i][0] != releationR[i][1]){
                    found = makeSet(newR[0], newR[1], found);
                    found2 = makeSet(releationR[i][0],releationR[i][1],found2);
                    found3 = makeSet(releationR[k][0],releationR[k][1],found3);
                    flag3 = 1;
                }

                /*for releations with the same 1st and 2nd indices*/
                else if(!flag3){
                    if(flag2){
                        found = makeSet(releationR[i][0], releationR[i][1], found2);
                        found2 = makeSet(releationR[i][0],releationR[i][1],found2);
                        found3 = makeSet(releationR[k][0],releationR[k][1],found3);
                    }
                }
            }
        }
    }
    if(result == allTrans){
        str = "Transitive : yes, " + found + "is found, since " + found2 + " and " + found3 + " are found. \n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
    else{
        str = "Transitive : no, " + notFound + " is not found " + " dispute there are " + notFound2 + " and " + notFound3 + "\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
}

void releation :: checkSymmetry(){

    int flag = 0, size=0, flag2 = 0, flag3=0;
    string str,found,notFound,found2,notFound2;

    for(int i=0;releationR[i][0] != '.';i++){
        flag2 = 0;
        for(int k=0;releationR[k][0] != '.';k++){
            /*for ex: for (a,b) and (b,a) a == a && b == b*/
            if(releationR[i][0] == releationR[k][1] && releationR[i][1] == releationR[k][0]){
                flag2 = 1;
                flag++;    /*if it is symetric then symetric element number is incremented by one*/  
            }
        }

        /*if it is not symetric, then i stored the elements that is not symetric*/
        if(!flag2){
            found = makeSet(releationR[i][0], releationR[i][1], found);
            notFound = makeSet(releationR[i][1], releationR[i][0], notFound);
        }
        /*if it is symetric, then i stored the elements that is symetric*/
        else if(flag2 && releationR[i][0] != releationR[i][1]){
            found2 = makeSet(releationR[i][0], releationR[i][1], found2);
            notFound2 = makeSet(releationR[i][1], releationR[i][0], notFound2);
            flag3 = 1;
        }
        /*for releations with the same 1st and 2nd indices*/
        if(!flag3){
            if(flag2){
                found2 = makeSet(releationR[i][0], releationR[i][1], found2);
                notFound2 = makeSet(releationR[i][1], releationR[i][0], notFound2);
            }
        }
        size++;     /*set size*/
    }
    /*each set has symmetry up to its size*/
    if(flag == size){
        str = "Symetric : yes, " + found2 + " is found whereas " + notFound2 + " is found\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
    else{
        str = "Symetric :no, " + found + " is found whereas " + notFound + " is not found\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
}

void releation :: checkAntiSymmetry(){

    int flag, flag2 = 0 ,flag3=0, flag4 = 0;
    string str,found,notFound,found2,notFound2;

    for(int i=0;releationR[i][0] != '.';i++){
        flag = 0;
        for(int k=0;releationR[k][0] != '.';k++){
            /*for ex: for (a,b) and (b,a) a == a && b == b*/
            if(releationR[i][0] == releationR[k][1] && releationR[i][1] == releationR[k][0])
                flag = 1;
        }

        /*if it is antisymetric, then i stored the elements that is antisymetric*/
        if(!flag){
            found = makeSet(releationR[i][0], releationR[i][1], found);
            notFound = makeSet(releationR[i][1], releationR[i][0], notFound);
            flag2 = 1;
        }

        /*if it is not antisymetric, then i stored the elements that is not antisymetric*/
        else if(flag && releationR[i][0] != releationR[i][1]){
            found2 = makeSet(releationR[i][0], releationR[i][1], found2);
            notFound2 = makeSet(releationR[i][1], releationR[i][0], notFound2);
            flag3 = 1;
        }

        /*for releations with the same 1st and 2nd indices*/
        if(!flag3 && flag){
            found2 = makeSet(releationR[i][0], releationR[i][1], found2);
            notFound2 = makeSet(releationR[i][1], releationR[i][0], notFound2);
        }
    }
    /*each set has symmetry up to its size so if symmetry element number is not equal to size , then this set is antisymetric*/
    if(flag2 && !flag3){
        str = "Antisymetric : yes, "  + found + " is found whereas " + notFound + " is not found\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
    else if(!flag3){
        str = "Antisymetric : yes, "  + found2 + " is found whereas " + notFound2 + " is not found\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }    
    else{
        str = "Antisymetric : no, " + found2 + " is found whereas " + notFound2 + " is found\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
}
void releation :: checkReflexivity(){

    int flag=0,flag2=0,i,k;
    string str,found,found2;

    for(i=0;i<elements.size();i++){
        flag2 = 0;
        for(k=0;releationR[k][0] != '.';k++){
            /*for ex : (a,a) ; if elements[i] == 'a' then it compares a==a, and a==a*/
            if(elements[i] == releationR[k][0] && elements[i] == releationR[k][1]){
                flag2=1;
                flag++;
            }
            if(flag2)
                found2 = makeSet(releationR[k][0],releationR[k][1],found2);

        }
        if(!flag2)
            found = makeSet(elements[i],elements[i],found);
    }
    /*each set has reflexive elements up to its size*/
    if(flag == elements.size()){
        str = "\nReflexive : yes, since " + found2 + " and " "all elements are present.\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
    else{
        str = "\nReflexive : no, since " + found + " is not present.\n";
        fileElemnt.insert(fileElemnt.length(),str);
    }
}

/*writing to output.txt file*/
void releation :: writeFile(int flag){

    ofstream outStream;

    /*it creat file and writes*/
    if (!flag)
        outStream.open("output.txt");
    /*it appends to the created file*/
    else 
        outStream.open("output.txt", std::ios_base::app);

    outStream << fileElemnt;
    releationR.clear();
    elements.clear();
    outStream.close();
}

/*this is a helper function that makes a releation. for ex : (a,c)*/
string releation :: makeSet(char first, char second, string found){

    found.resize(5);
    found[0] = '(';
    found[1] = first;
    found[2] = ',';
    found[3] = second;
    found[4] = ')';

    return found;
}