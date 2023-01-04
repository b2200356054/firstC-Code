#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

int dotProductForMatrices(int **arr, int **arr2, int keySize){
    int sum = 0;
    int firstNumber;
    int secondNumber;

    for (int i = 0; i < keySize; i++){
        for (int j = 0; j < keySize; j++) {
            firstNumber = arr[i][j];
            secondNumber = arr2[i][j];
            sum += firstNumber*secondNumber;
        }
    }
    return sum;
}

int *newCoordinates = new int[2];
string rowWrite, columnWrite, resultWrite, writeStr;
vector<string> searchVector;
void searchForTreasure(int** theMap, int mapRowSize, int mapColumnSize, int **keyMatrix, int keySize,
                       vector<string> &vector1,  int coordinates[2], int startLocInt){
    /*SEARCH ALGORITHM
     * START FROM THE FIRST SQAURE
     * START FROM THE COORDINATES PASSED IN THE ARGUMENTS
     * DOT PRODUCT THE GIVEN MATRIX AND THE KEY,
     * RECORD THE COORDINATES AND THE PRODUCT TO OUTPUTVECTOR, THEN USE THE REMAINDER AS THE NEXT
     * TO INCREMENT OR DECREMENT THE COORDINATES*/
    int remainder;
    bool found = false;
    int row = coordinates[0];
    int column = coordinates[1];
    newCoordinates[0] = row;
    newCoordinates[1] = column;


    //SUB MATRIX DECLARATION
    int** submatrix = new int*[keySize];
    for (int i = 0; i < keySize; i++){
        submatrix[i] = new int[keySize];
    }
    int a;
    for (int i = 0; i < keySize; i++){
        for (int j = 0; j < keySize; j++){
            a = theMap[row-startLocInt+i][column-startLocInt+j];
            submatrix[i][j] = a;
        }
    }
    int result = dotProductForMatrices(submatrix, keyMatrix, keySize);
    while (result < 0){
        result += 5;
    }
    remainder = result % 5;



    //DELETION OF THE CREATED DYNAMIC SUB_MATRIX
    for (int i = 0; i < keySize; i++){
        delete[] submatrix[i];
    }
    delete[] submatrix;


    //REMAINDER 0 MEANS TREASURE IS FOUND
    if (remainder == 0){
        found = true;

    //REMAINDER 1 MEANS GO UP
    } else if (remainder == 1) {
        if (row - keySize >= 1){
            newCoordinates[0] = row - keySize;
        } else {
            newCoordinates[0] = row + keySize;
        }

    // REMAINDER 2 MEANS GO DOWN
    } else if (remainder == 2) {
        if (row + keySize <= mapRowSize-2){
            newCoordinates[0] = row + keySize;
        } else {
            newCoordinates[0] = row - keySize;
        }

    // REMAINDER 3 MEANS GO RIGHT
    } else if (remainder == 3) {
        if (column + keySize <= mapColumnSize-2){
            newCoordinates[1] = column + keySize;
        } else {
            newCoordinates[1] = column - keySize;
        }

    // REMAINDER 4 MEANS GO LEFT
    } else if (remainder == 4)
        if (column - keySize >= 1){
            newCoordinates[1] = column - keySize;
        } else {
            newCoordinates[1] = column + keySize;
        }


    rowWrite = to_string(row);
    columnWrite = to_string(column);
    resultWrite = to_string(result);
    writeStr = rowWrite+","+columnWrite+":"+resultWrite+"\n";
    vector1.push_back(writeStr);
    if (!found){
        searchForTreasure(theMap, mapRowSize, mapColumnSize, keyMatrix, keySize, vector1, newCoordinates, startLocInt);
    }
}
vector<string> stringSplitterToArray(const string& stringToSplit, const char *delimiter){
    vector<string> vectr;
    char *stringToSplit1 = const_cast<char*>(stringToSplit.c_str());
    char *ptr = strtok(stringToSplit1, delimiter);
    while (ptr != nullptr){
        vectr.emplace_back(ptr);
        ptr = strtok(nullptr, delimiter);
    }
    return vectr;

}
int main(int argc, char *argv[]) {
    //Recorded all arguments
    vector<string> allArgs(argv, argv+argc);

    //Recorded Map Size Here
    string mapSize1;
    mapSize1 = allArgs[1];

    char mapSize[mapSize1.size()];
    strcpy(mapSize, mapSize1.c_str());

    vector<string> mapSizeVector =
            *new vector<string>(stringSplitterToArray(mapSize, "x"));


    //INITIALIZED ROW AND COLUMN NUMBERS
    int rowNum = stoi(mapSizeVector[0]);
    int columnNum = stoi(mapSizeVector[1]);
    int keySize = stoi(allArgs[2]);
    int start = keySize/2;
    //INITIALIZED FILE NAMES ACCORDING TO CMDLINE ARGUMENTS
    string mapFileName;
    mapFileName = allArgs[3];
    string keyFileName;
    keyFileName = allArgs[4];
    string outputFileName;
    outputFileName = allArgs[5];

    //DECLARATION OF THE MAP ARRAY DYNAMICALLY
    int** theMap = new int*[rowNum];
    //INITIALIZING THE MAP MATRIX
    for (int i = 0; i < rowNum; i++){
        theMap[i] = new int[columnNum];
    }

    //INPUT FOR MAP
    ifstream mapFile(mapFileName);
    string mapRows;

    ////INITIALIZING THE MAP PROPERLY FROM THE MAP MATRIX FILE
    int copyInt = 0;
    while (getline(mapFile, mapRows)){
        vector<string> mapRowVector = stringSplitterToArray(mapRows, " ");
        //FOR LOOP HERE
        for (int i = 0; i < mapRowVector.size(); i++){
            theMap[copyInt][i] = stoi(mapRowVector[i]);

        }

        (copyInt)++;
    }

    //DECLARATION OF THE KEY ARRAY
    int **keyMatrix = new int*[keySize];
    //INITIALIZING KEY MATRIX AS NxN
    for (int i = 0; i < keySize; i++){
        keyMatrix[i] = new int[keySize];
    }

    //COPYING KEY TO AN ARRAY FROM A FILE
    string keyRows;
    copyInt = 0;
    ifstream keyFile(keyFileName);
    while (getline(keyFile, keyRows)){
        vector<string> keyRowVector = stringSplitterToArray(keyRows, " ");

        //FOR LOOP HERE
        for (int i = 0; i < keyRowVector.size(); i++){
            keyMatrix[copyInt][i] = stoi(keyRowVector[i]);

        }
        copyInt++;

    }

    int startLoc[2] = {start ,start};
    searchForTreasure(theMap, rowNum, columnNum, keyMatrix, keySize,
                      searchVector, startLoc, start);
    //DELETING THE MAP
    for (int i = 0; i < rowNum; i++){
        delete[] theMap[i];
    }
    delete[] theMap;


    //DELETING THE KEY
    for (int i = 0; i < keySize; i++){
        delete[] keyMatrix[i];
    }
    delete[] keyMatrix;


    ofstream outputFile(outputFileName);
    for (const string& i : searchVector){
        outputFile << i;
    }
    outputFile.close();
    return 0;
}
