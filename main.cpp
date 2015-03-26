#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <regex>
#include <stdlib.h>

#include ".//lib//tinyxml.h"
#include ".//lib//tinystr.h"


using namespace std;

// var name ...
enum {
    LOAD_DATA_FAIL = -1,
    MODE_FAIL = -1,
    LOAD_DATA_SUCCESS = 0,
    
    RW_CSV_MODE = 0,
    CHECK_FORMAT_MOD = 1,
    ANDROID_STRING_2_IOS_STRING = 2,
    LAST_MODE
};

FILE *_File;
string strInputFileName;
int gDataNum;
vector<vector<string> > gvDataBuff;
int gSwitchMode;


// function ...
void inputFileName();
int loadFileCSVMode();
void split(char **arr, char *str, const char *del);
string getFilePath(const std::string& str);
string int2str(int &i);
void saveData();
void handleSaveOneFile(int index, FILE *&f);
int inputSwitchMode();
int loadDataCheckMode();
void refreshMain();

void LoadXml(string file);

int main(int argc, char const *argv[])
{
    while (1) {
        cout<<"Switch mode:\n\t(0.) Export string-table by CSV file.\n\t(1.) Check string-table format.\n\t(2.) Translate string.xml to xcode string."<<endl<<endl;
        inputSwitchMode();
        system("read -p \"please enter\"");
        refreshMain();
    }
    return 0;
}

void inputFileName()
{
    cout<<"input your file path:";
    cin>>strInputFileName;
}

int loadFileCSVMode()
{
    ifstream fin(strInputFileName);
    
    if (!fin) {
        cout << "open data fail." << endl;
        return LOAD_DATA_FAIL;
    } else {
        string str_buf;
        
        while (getline(fin, str_buf,'\r')) {
            istringstream stream(str_buf);
            vector<string> vStr;
            
            while (getline(stream, str_buf, ',')) {
                cout<<str_buf<<" ";
                vStr.push_back(str_buf);
            }
            gvDataBuff.push_back(vStr);
            cout<<endl;
        }
        
        fin.close();
        fin.clear();
    }
    return LOAD_DATA_SUCCESS;
}

void split(char **arr, char *str, const char *del)
{
    char *s = strtok(str, del);
    
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
    }
}

void SplitFilename (const std::string& str)
{
    std::cout << "Splitting: " << str << '\n';
    unsigned long found = str.find_last_of("/\\");
    std::cout << " path: " << str.substr(0,found) << '\n';
    std::cout << " file: " << str.substr(found+1) << '\n';
}

string getFilePath(const std::string& str)
{
    unsigned long found = str.find_last_of("/\\");
    return str.substr(0,found);
}

string int2str(int &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    
    return ss.str();
}

void saveData()
{
    string strPath = getFilePath(strInputFileName);
    FILE *file[gvDataBuff.size()-1];
    for (int i=1; i<gvDataBuff[0].size(); i++) {
        string dstPath = strPath+'/'+int2str(i);
        
        // open file
        file[i-1] = fopen(dstPath.c_str(), "a");
        
        // log
        cout<<"[Info]["<<i<<"] the file path is "<<dstPath<<endl;
        
        // save file
        handleSaveOneFile(i, file[i-1]);
    }
    
    // close file
    for (int i=0; i<gvDataBuff[0].size()-1; i++) {
        fclose(file[i]);
    }
    
}

void handleSaveOneFile(int index, FILE *&f)
{
    for (int i=0; i<gvDataBuff.size(); i++) {
        string strKey = gvDataBuff[i][0];
        string strCol = gvDataBuff[i][index];
        
        fprintf(f, "\"%s\"=\"%s\";\r", strKey.c_str(), strCol.c_str());
    }
}

int inputSwitchMode()
{
    cout<<"input mode [0-"<<LAST_MODE-1<<"]:";
    cin>>gSwitchMode;
    cout<<endl;
    
    switch (gSwitchMode) {
        case RW_CSV_MODE:
        {
            cout<<"Programming start ..."<<endl;
            inputFileName();
            
            if (LOAD_DATA_SUCCESS == loadFileCSVMode()) {
                saveData();
            }
            cin.ignore().get();
            return RW_CSV_MODE;
        }
            break;
        case CHECK_FORMAT_MOD:
        {
            cout<<"Programming start ..."<<endl;
            inputFileName();
            loadDataCheckMode();
            return CHECK_FORMAT_MOD;
        }
            break;
            
        case ANDROID_STRING_2_IOS_STRING:
            cout<<"Programming start ..."<<endl;
            inputFileName();
            LoadXml(strInputFileName);
            
            return ANDROID_STRING_2_IOS_STRING;
            break;
            
        default:
            cout<<"input error"<<endl;
            return MODE_FAIL;
            break;
    }
    return MODE_FAIL;
}

int loadDataCheckMode()
{
    ifstream fin(strInputFileName);
    
    if (!fin) {
        cout << "open data fail." << endl;
        return LOAD_DATA_FAIL;
    } else {
        string str_buf;
        unsigned int i = 1;
        while (getline(fin, str_buf,'\n')) {
            
            regex integer("\"+[a-zA-Z0-9_]+\"+[=| = | =|= ]+\"+(.*\n*.*){0,4}\"+[;]");
            
            if(regex_match(str_buf,integer)) {
                
            } else {
                cout<<"Invalid input line:["<<i<<"] "<<str_buf<<endl;
            }
            i ++;
        }
        
        fin.close();
        fin.clear();
    }
    return LOAD_DATA_SUCCESS;
}

void refreshMain()
{
    for (int i=0; i<20; i++) {
        cout<<endl;
    }
}

void LoadXml(string file)
{
    // get file path
    string path = getFilePath(file);
    path.append("//string");
    cout<<"[Info] The file path is "<<path<<endl;
    
    // open file
    FILE *ff = fopen(path.c_str(), "a");
    
    // handle xml file
    TiXmlDocument *myDocument = new TiXmlDocument(file.c_str());
    myDocument->LoadFile();
    
    TiXmlElement *RootElement = myDocument->RootElement();
    cout << RootElement->Value() << endl;
    
    TiXmlElement *FirstPerson = RootElement->FirstChildElement();
    TiXmlElement *next = FirstPerson;
    while (next != NULL) {
        TiXmlAttribute *IDAttribute = next->FirstAttribute();
        TiXmlElement *ValueElement = next;
        
        // cout<<"\""<<IDAttribute->Value()<<"\"=\""<<ValueElement->FirstChild()->Value()<<"\";"<<endl;
        
        string strKey = IDAttribute->Value();
        string strCol = ValueElement->FirstChild()->Value();
        
        // write data to file
        fprintf(ff, "\"%s\"=\"%s\";\r", strKey.c_str(), strCol.c_str());
        
        next = next->NextSiblingElement();
    }
    
    // close file
    fclose(ff);
}










