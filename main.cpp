/*-------------------------------------------------------------
  Student's Name: Boyang YAN
  Student's email address: by932@uowmail.edu.au
  Student user name: by932
  Assignment: Assignment one
  About: read a context and find word frequence and sort
  Last Modification: 05/08/2017
  -------------------------------------------------------------*/

#include<cstdio>
#include<cstring>
#include<iostream>
#include <fstream>
using namespace std;

void recordWordFrequence(char*);
int partition(int, int);
void quickSort (int, int);
int frequenceNodeCompareFirstArgBigger(int&, int&);
//trie node for count word frequence
struct TrieNode
{
    int children[26];
    int isLeaf;//if isLeaf >= 0 mean is end node and that number is point to story frequence array(string pool). if isLeaf is -1 mean not end node
};
//frequenceNode for string pool
struct frequenceNode
{
    char word[20];
    unsigned int frequence;
};
TrieNode trieArray[1000000];//for story trie
unsigned int trieArrayIndex = 1;//count trie size

frequenceNode frequenceRecordArray[50000];//for story string pool
int wordRecordCount = 0;//number of string in string pool

int main(){
    //init trie root node
    for (int i = 0; i < 26; ++i) {
        trieArray[0].children[i] = 0;
    }
    trieArray[0].isLeaf = -1;

    //read the name of the text file from console
    char fileName[20];
    cout << "Please input file name for open: ";
    cin >> fileName;

    //open file
    ifstream file (fileName);
    if (file.is_open()) {
        char line[50];
        char word[20];
        //read line by line
        while (file.getline (line, 50)) {
            if (!file.good()) {
                return 0;
            }
            //extracting words from a line of text
            for(int n = 0, m = 0; m + 1 < static_cast<int>(strlen( line )); m += n){
                sscanf(&line[m], "%s%n", word, &n);
                recordWordFrequence(word);//story the unique words and maintain a count of each different word
            }
        }
        //using quickSort sort string pool in decreasing count and same count by alphabetically
        quickSort (0, wordRecordCount - 1);

        //print frist ten words and last ten words in sorted list with word frequence
        for (int i = 0; i < wordRecordCount; ++i) {
            cout << frequenceRecordArray[i].word << " ";
            cout << frequenceRecordArray[i].frequence << " | ";
            if (i == 10){
                i = wordRecordCount -11;
                cout << endl;
            }
        }
        cout << endl;
    }
    else {
        cout << "Unable to open file" << endl;
    }
    return 0;
}
//set up trie with unique word and word frequence
void recordWordFrequence(char * word)
{
    int count = 0;
    int childIndex = 0;
    for (int level = 0, index = 0; level < static_cast<int>(strlen(word)); level++)
    {
        //change capital case to lower case
        if (word[level] >= 65 && word[level] <= 90){
            word[level] += 32;
        }
        //discarding punctuation
        if (word[level] >= 97 && word[level] <= 122){
            index = static_cast<int>(word[level]) - 97;
            //root node
            if (level == 0){
                if (trieArray[0].children[index] == 0){//no child in root
                    //set up a new node
                    trieArray[0].children[index] = trieArrayIndex++;
                    childIndex = trieArray[0].children[index];
                }
                else
                    childIndex = trieArray[0].children[index];
            }
            else {
                if (trieArray[childIndex].children[index] > 0)//have child
                    childIndex = trieArray[childIndex].children[index];
                else {//not child, creat a new child
                    trieArray[trieArrayIndex].isLeaf = -1;
                    trieArray[childIndex].children[index] = trieArrayIndex++;
                    childIndex = trieArray[childIndex].children[index];
                }
            }
            word[count++] = word[level];
        }
    }
    word[count] = '\0';
            //!word[0] =='\0' for avid some word have not alphabet
            if (!(word[0] == '\0')) {
                //word exist, omly increase frequence
                if (trieArray[childIndex].isLeaf >= 0) {
                    frequenceRecordArray[trieArray[childIndex].isLeaf].frequence++;
                }
                //word not exist in frequenceRecordArray, creat a new one and let isLeaf point into it
                else {
                    trieArray[childIndex].isLeaf = wordRecordCount++;
                    frequenceRecordArray[trieArray[childIndex].isLeaf].frequence = 1;
                    strcpy (frequenceRecordArray[trieArray[childIndex].isLeaf].word, word);
                }
    }

    return;
}
//for compare two element in string pool. if return 0 mean one = two. if return 1 mean one > two. if return -1 mean one < two
int frequenceNodeCompareFirstArgBigger(int& one, int& two)
{
    if (one == two)
        return 0;
    if (frequenceRecordArray[one].frequence > frequenceRecordArray[two].frequence)
        return 1;
    else if (frequenceRecordArray[one].frequence == frequenceRecordArray[two].frequence) {
        if (strcmp(frequenceRecordArray[one].word, frequenceRecordArray[two].word) > 0)
            return -1;
        else
            return 1;
    }
    else
        return -1;
}
//using in quickSort
int partition (int left, int right)
{
    int val = left;
    int lm = left - 1;
    int rm = right + 1;

    for (;;)
    {
        do
        {
            rm--;
        }while (frequenceNodeCompareFirstArgBigger(rm, val) == -1);//rm < val
        do
        {
            lm++;
        }while (frequenceNodeCompareFirstArgBigger(lm, val) == 1);//lm > val
        if (lm < rm)
        {
            //swap
            frequenceNode tempr = frequenceRecordArray[rm];
            frequenceRecordArray[rm] = frequenceRecordArray[lm];
            frequenceRecordArray[lm] = tempr;
        }
        else
            return rm;
    }
}
void quickSort (int left, int right)
{
    if (left < right)
    {
        int splitPt = partition(left, right);
        quickSort(left, splitPt);
        quickSort(splitPt + 1, right);
    }
    return;
}
