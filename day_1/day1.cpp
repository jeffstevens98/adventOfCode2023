//Day 1 of advent of code 2023
//Problem: https://adventofcode.com/2023/day/1

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cctype>
#include<unordered_map>
#include<algorithm>
#include<chrono>


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
 *  std::vector<int> - A vector containing all indices in increasing order that the substr occurs at.
*/
std::vector<int> findAll(    std::string str,
                                std::string substr  )
{
    std::vector<int> positions;

    auto pos = str.find(substr, 0);
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
int findCalibrationValueSum(char * fileName)
{
    //Open the file
    std::fstream file(fileName);
    std::cout << "Opening calibration document...\n";
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << fileName << "'";
        exit(EXIT_FAILURE);
    }
    //File is ready!
    std::cout << "Succesfully opened calibration document!\n";

  
    std::vector<int> numbersInLine = {};
    int lineCode{0}, sum{0};
    std::string line;
    //For each line of the calibration document, iterate through the characters
    while( getline(file,line) )
    {
        std::cout << "Current line: " << line << "\n";
        for(const char& currChar : line)
        {
            std::cout << "Current char: " << currChar << "\n";
            //If we find a number, add it to our list of numbers in the line
            if( isdigit(currChar) )
            {
                std::cout << "Found number: " << currChar << "\n";
              
                // We subtract '0' as that gives us the non-ASCII value when converting from a char to an int
                // See https://stackoverflow.com/a/5030086
                numbersInLine.push_back(currChar - '0');
            }
        }
        std::cout << "Finished looking through all chars in line\n";
        // We take the first element of numbersInLine and turn it into a multiple of ten, and then add the last element (10x + y)
        lineCode = 10 * (*numbersInLine.begin()) + *(numbersInLine.end() - 1);
        std::cout << "Line code found: " << lineCode << "\n"; 
        // Add it to a sum
        sum += lineCode;
        //Clear the current numbers in the line
        numbersInLine.clear();
    }

    //Close the file
    file.close();
    
    return sum;
}


/**
 * Second part of the first advent of code problem.
*/
int findCalibrationValueSum_v2(char * fileName)
{
    //Open the file
    std::fstream file(fileName);
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << fileName << "'";
        exit(EXIT_FAILURE);
    }

    //File is ready!
    std::unordered_map<std::string,int> thingsWeAreLookingFor = { {"one",     1},
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

    std::string line;
    int lineCode{0}, sum{0};
    while( getline(file, line) )
    {
        //Resize the current numbersAtIndices vector for the line, and fill it with -1
        std::vector<int> numbersAtIndices(line.length(), -1);
      
        //Search the vector for the string numbers and number names that we're looking for
        for(const auto & [key,value] : thingsWeAreLookingFor)
        {
            //Search the line for all occurrences of the key
            std::vector<int> foundPositions = findAll(line, key);
            //For every occurrence we have, insert the string value of the number at the appropriate index in numbersAtIndices
            for (const int& pos : foundPositions) {
              numbersAtIndices.at(pos) = value;
            }
        }
        numbersAtIndices = eraseAll(numbersAtIndices, -1);
        //Now that we have all the numbers at each index in the line, find the first and last
        lineCode = 10 * (*numbersAtIndices.begin()) + *(numbersAtIndices.end() - 1);
        //Add it to sum
        sum += lineCode;
    }

    //Close the file
    file.close();

    return sum;
}


//Main function
int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        std::cout << "Starting timetest of aoc 2023 day 1!\n";
        float countSum = 0;
        for(int i = 0; i < 1000; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            findCalibrationValueSum_v2(argv[1]);
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            countSum += duration.count();
            std::cout << duration.count() << std::endl;
        }
        std::cout << "Average day 1 p2 function execution time: " << (countSum / 1000) << "\n";
}
    else if(argc > 1)
    {
        int sum = findCalibrationValueSum_v2(argv[1]);
        std::cout << sum << "\n";
    }
    else
    {
        std::cout << "You must enter the name of the calibration document's file as the first argument for the program.\n";
    }

    return 0;
}
