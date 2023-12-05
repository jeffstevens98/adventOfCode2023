//Day 1 of advent of code 2023
//Problem: https://adventofcode.com/2023/day/1

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cctype>
#include<map>
#include<algorithm>


/**
 * Given a string str, find all occurrences of a substring within it. Returns a vector of all of the indices that the substring
 * occurs at within the string str.
 * 
 * Based off of: https://stackoverflow.com/a/4034809
 * 
 * Parameters:
 *  std::string str - The string we are searching for the substring in.
 *  std::string substr - The substring we are looking for within string str.
 * 
 * Returns:
 *  std::vector<size_t> - A vector containing all indices in increasing order that the substr occurs at.
*/
std::vector<size_t> findAll(    std::string str,
                                std::string substr  )
{
    std::vector<size_t> positions;

    size_t pos = str.find(substr, 0);
    while(pos != std::string::npos)
    {
        positions.push_back(pos);
        pos = str.find(substr,pos+1);
    }

    return positions;
}


/**
 * Given a vector of objects T vec, and an eraseTarget of type T, erase all instances of the eraseTarget 
 * from vec, then return vec.
 * 
 * Parameters:
 *  std::vector<T> vec - The vector we are erasing all of an eraseTarget from.
 *  T eraseTarget - The thing we are erasing all occurrences of in vec.
 * 
 * Returns:
 *  std::vector<T> - The modified vector vec with all instances of eraseTarget erased from it.
*/
template<typename T>
std::vector<T> eraseAll(    std::vector<T> vec,
                            T eraseTarget)
{
    vec.erase(std::remove(vec.begin(), vec.end(), eraseTarget), vec.end());
    return vec;
}


/**
 * Given the scrambled calibration document, find the calibration code for each line and find the sum of all of them. 
 * Prints the sum to the console with std::cout.
 * 
 * Parameter:
 *  char * fileName - The name of the calibration document's file on the computer.
 * 
 * Returns:
 *  void
*/
void findCalibrationValueSum(char * fileName)
{
    //Open the file
    std::fstream file;
    std::cout << "Opening calibration document..." << std::endl;
    file.open(fileName);
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << fileName << "'";
    }

    //File is ready!
    std::cout << "Succesfully opened calibration document!" << std::endl;
    std::string line;
    std::string currCharAsStr;
    std::vector<std::string> numbersInLine = {};
    std::vector<int> lineCodes = {};
    int lineCode;
    //For each line of the calibration document, iterate through the characters
    while( getline(file,line) )
    {
        std::cout << "Current line: " << line << std::endl;
        for(int i=0; i < line.size(); i++)
        {
            std::cout << "Current char: " << line[i] << std::endl;
            //If we find a number, add it to our list of numbers in the line
            if( isdigit(line[i]) )
            {
                std::cout << "Found number: " << line[i] << std::endl;
                std::string s(1,line[i]);
                numbersInLine.push_back(s);
            }
        }
        std::cout << "Finished looking through all chars in line" << std::endl;
        //Once we've finished looking through all the characters in the line, take the first and last numbers of the numbersInLine vector and concatenate them
        lineCode = std::stoi(numbersInLine.front() + numbersInLine.back());
        std::cout << "Line code found: " << lineCode << std::endl; 
        //Add that as the line code
        lineCodes.push_back(lineCode);
        //Clear the current numbers in the line
        numbersInLine.clear();
    }

    //Close the file
    file.close();

    //Now that we have the calibration codes for each line, let's sum them all up
    int sum = 0;
    for(int i=0; i < lineCodes.size(); i++)
    {
        sum += lineCodes[i];
    }

    //Print the result:
    std::cout << "Sum of calibration codes: " << sum << std::endl;
}


/**
 * Second part of the first advent of code problem.
*/
void findCalibrationValueSum_v2(char * fileName)
{
    //Open the file
    std::fstream file;
    //std::cout << "Opening calibration document..." << std::endl;
    file.open(fileName);
    // if(!file.is_open())
    // {
    //     std::cout << "There was a problem and we were unable to open a file with the name:'" << fileName << "'";
    // }

    //File is ready!
    //std::cout << "Succesfully opened calibration document!" << std::endl;
    std::string line;
    std::vector<int> lineCodes = {};
    int lineCode;
    std::vector<int> numbersAtIndices;
    std::vector<size_t> foundPositions;
    std::map<std::string,int> thingsWeAreLookingFor = { {"one",     1},
                                                        {"two",     2},
                                                        {"three",   3},
                                                        {"four",    4},
                                                        {"five",    5},
                                                        {"six",     6},
                                                        {"seven",   7},
                                                        {"eight",   8},
                                                        {"nine",    9},
                                                        {"1",       1},
                                                        {"2",       2},
                                                        {"3",       3},
                                                        {"4",       4},
                                                        {"5",       5},
                                                        {"6",       6},
                                                        {"7",       7},
                                                        {"8",       8},
                                                        {"9",       9}  };
    while( getline(file, line) )
    {
        //std::cout << "Current line: " << line << std::endl;
        //Resize the current numbersAtIndices vector for the line
        numbersAtIndices.resize(line.length());
        //std::fill(numbersAtIndices.begin(), numbersAtIndices.end(), -1);
        //Search the vector for the string numbers and number names that we're looking for
        for(const auto & [key,value] : thingsWeAreLookingFor)
        {
            //Search the line for all occurrences of the key
            foundPositions = findAll(line, key);
            //For every occurrence we have, insert the string value of the number at the appropriate index in numbersAtIndices
            for(int i=0; i < foundPositions.size(); i++)
            {
                //std::cout << "Found \"" << key << "\" at index " << foundPositions[i] << std::endl;
                numbersAtIndices[foundPositions[i]] = value;
            }
        }
        numbersAtIndices = eraseAll(numbersAtIndices, -1);
        //Now that we have all the numbers at each index in the line, find the first and last
        lineCode = std::stoi(std::to_string(numbersAtIndices.front()) + std::to_string(numbersAtIndices.back()));
        //std::cout << "Line code found: " << lineCode << std::endl; 
        //Add that as the line code
        lineCodes.push_back(lineCode);
        //Clear the current numbers in the line
        numbersAtIndices.clear();
    }

    //Close the file
    file.close();

    //Now that we have the calibration codes for each line, let's sum them all up
    int sum = 0;
    for(int i=0; i < lineCodes.size(); i++)
    {
        sum += lineCodes[i];
    }

    //Print the result:
    std::cout << "Sum of calibration codes: " << sum << std::endl;
}


//Main function
int main(int argc, char *argv[])
{
    
    if(argc > 1)
    {
        findCalibrationValueSum_v2(argv[1]);
    }
    else
    {
        std::cout << "You must enter the name of the calibration document's file as the first argument for the program." << std::endl;
    }
    return 0;
}