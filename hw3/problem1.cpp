#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/* Author : Merve Horuz */

using namespace std;

class hasse{
public:

    void readFile();
    void parseElements(string& str);
    void parseReleations(string& str);
    bool validElement(int cnt, const string& str);
    void addN();
    void addReflexivity(int i);
    void addTransitivity(int order);
    void removeSymetry(int order);
    void removeReflexive(int order);
    void removeTransitive(int order);
    void writeFile(int order);
    void addFileContentsToFileElements(int order);

private:
    vector<vector<char> > elements;    /*elements in each releations. (a,b,c...)*/
    vector<vector<char> > releations;    /*relelations in each set*/
    vector<vector<char> > hasseElement;    /*hasse diagram in each set*/
    string fileElement;    /*elements which will write to file*/
};

int main(){

    hasse poset;

    poset.readFile();

    for(int i=0;i<8;i++){
        
        poset.addN();
        poset.addReflexivity(i);
        poset.removeSymetry(i);
        poset.addTransitivity(i);
        poset.removeReflexive(i);
        poset.removeTransitive(i);
        poset.writeFile(i);
    }

    return 0;
}

/* to write 'n' and 'POSET' to beginning of each set */
void hasse :: addN(){

    fileElement.clear();
    fileElement = "n\nPOSET : ";
}

void hasse :: readFile(){

    ifstream inStream;
    inStream.open("input.txt");    /* opening file */
    string str,relStr;
    int flag;

    if(inStream.fail())
        cerr << "error opening the file : input.txt";

    else{

        getline(inStream,str);      /*taking one line from file (for first indicator)*/

        for(int i=0;!inStream.eof();i++){

            getline(inStream,str);    /* taking elements of each set */
            parseElements(str);    /*deleting comma and newline character */
            relStr.clear();

            flag = 1;    /* condition of loop */
            for(int k=0;flag;k++){

                getline(inStream,str);    /* taking releations */
                if(validElement(i,str))
                    relStr += str;    /* if releation is valid, then releation is stored in another string */
                else
                    flag = 0;    /* loop is terminated */
            }
            parseReleations(relStr);
        }
    }
    inStream.close();
}

/*in this algorithm, comma and newline are deleted in elements set*/
void hasse :: parseElements(string& str){

    vector <char> temp;

    for(int i=0;str[i] != '\0'; i++)
        if(str[i] != ',' && str[i] != '\n')
            temp.push_back(str[i]);

    elements.push_back(temp);    /* storing in elements vector */
}

/* it checks whether relelation is true form and true value or not */
bool hasse :: validElement(int cnt, const string& str){

    for(int i=0;i<str.length();i++)
        if(str[i] == ',')   /* like a,b */
            return true;
    
    return false;
}

/*in this algorithm, comma and newline are deleted in releation */
void hasse :: parseReleations(string& str){

    vector <char> temp;

    for(int i=0;str[i] != '\0'; i++)
        if(str[i] != ',' && str[i] != '\n')
            temp.push_back(str[i]);

    releations.push_back(temp);
}

void hasse :: addReflexivity(int i){

    int flag = 0;
    string temp,temp2;

    for(int k=0;k<elements[i].size();k++){    /* it checks that whether there is reflexive releation or not, one by one for each elements */
        flag = 0;
        for(int j=0;j<releations[i].size();j += 2){
            if(elements[i][k] == releations[i][j] && elements[i][k] == releations[i][j+1])    
                flag = 1;    /* if relelation[i][j] == relelation[i][j+1] then this releation is reflexive and flag is true*/
        }

        /* if this element (anything) is not reflexive,,,*/
        if(!flag){

            temp2.resize(3);
            /* for this form : a,b */
            temp2[0] = elements[i][k];
            temp2[1] = ',';
            temp2[2] = elements[i][k];
    
            temp = "(" + temp2 + ")";    /* temp is --> (a,b) */
            fileElement.insert(fileElement.length(),temp);    /* adding fileElement */
        }
    }
}

void hasse :: addTransitivity(int order){

    string temp,temp2;
    int flag=0;

    for(int i=0;i<releations[order].size();i+=2){    /* for each releation */

        temp.clear();
        temp[0] = releations[order][i];    /* ith releation's first element */
        temp[1] = releations[order][i+1];    /* ith releation's second element */

        for(int k=0;k<releations[order].size() && temp[0] != temp[1];k+=2){

            flag = 0;
            /* if temp is a,b and there is releation that start with b*/
            if(temp[1] == releations[order][k]){

                for(int j=0;j<releations[order].size();j+=2)
                    if(temp[0] == releations[order][j] && releations[order][j+1] == releations[order][k+1])    /* if there is transitive of (for example) a,b and b,c*/
                        flag = 1;
                
                /* if there is no transitive of (for ex) a,b and b,c*/
                if(!flag){

                    temp2.resize(3);
                    temp2[0] = temp[0];
                    temp2[1] = ',';
                    temp2[2] = releations[order][k+1];

                    temp2 = "(" + temp2 + ")";
                    fileElement.insert(fileElement.length(),temp2);
                }
            }
        }
    }
}

void hasse :: removeSymetry(int order){

    int flag;

    for(int i=0;i<releations[order].size();i+=2){    /* variable i increment with two because each releataion has two elements */

        flag = 0;
        for(int k=0;k<releations[order].size();k+=2)
            if(releations[order][k] == releations[order][i+1] && releations[order][k+1] == releations[order][i] && releations[order][k] != releations[order][k+1]){
                /* if releations[order][i] == a and releations[order][i+1] == b and there is releation that b,a, then we delete releation b,a */
                releations[order][k] = '0';
                releations[order][k+1] = '0';
            } 
    }
}

/* for hasse diagram we remove reflexive releation */
void hasse :: removeReflexive(int order){

    vector<char> temp;
    
    for(int i=0;i<releations[order].size();i+=2){
        if(releations[order][i] != releations[order][i+1] && releations[order][i] != '0' && releations[order][i+1] != '0'){
            temp.push_back(releations[order][i]);
            temp.push_back(releations[order][i+1]);
        }
    }
    hasseElement.push_back(temp);    /* adding hasse elements to hasseElement vector's */
}

/* for hasse diagram we remove transitive elements from hasseElement vector's */
void hasse :: removeTransitive(int order){

    string temp;

    for(int i=0;i<hasseElement[order].size();i+=2){

        temp.clear();
        temp[0] = hasseElement[order][i];
        temp[1] = hasseElement[order][i+1];

        for(int k=0;k<hasseElement[order].size();k+=2){

            if(temp[1] == hasseElement[order][k]){

                for(int j=0;j<hasseElement[order].size();j+=2)
                    if(temp[0] == hasseElement[order][j] && hasseElement[order][j+1] == hasseElement[order][k+1]){

                        hasseElement[order][j] = '0';
                        hasseElement[order][j+1] = '0';
                    }   
            }
        }
    }
}

void hasse :: writeFile(int order){

    ofstream outStream;

    if(order == 0){    /* if file element is writed first time, then the output file is opened on creat and write mode */

        outStream.open("output.txt");
        addFileContentsToFileElements(order);
    }
    
    else{    /* if file element is writed but not first time, then the output file is opened on append mode */
        
        outStream.open("output.txt", std::ios_base::app);
        addFileContentsToFileElements(order);
    }

    outStream << fileElement;
}

/* in this algorithm, file elements are added to fileElement vector's */
void hasse :: addFileContentsToFileElements(int order){

    string temp;
    int i;

    /* for each releation */
    for(i=0;i<releations[order].size();i+=2){
            if(releations[order][i] != '0' && releations[order][i+1] != '0'){
                temp.resize(5);
                /* for this form --> (a,b) */
                temp[0] = '(';   
                temp[1] = releations[order][i];
                temp[2] = ',';
                temp[3] = releations[order][i+1];
                temp[4] = ')';
                fileElement.insert(fileElement.length(),temp);
                temp.clear();
            }     
        }

        /* for each hasse diagram */
        for(i=0;i<hasseElement[order].size();i+=2){
            if(hasseElement[order][i] != '0' && hasseElement[order][i+1] != '0'){
                temp.clear();
                temp.resize(6);
                temp[0] = '\n';
                temp[1] = '(';
                temp[2] = hasseElement[order][i];
                temp[3] = ',';
                temp[4] = hasseElement[order][i+1];
                temp[5] = ')';
                fileElement.insert(fileElement.length(),temp);
            }
        }
        fileElement.insert(fileElement.length(),"\n");
}