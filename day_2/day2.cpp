//Advent of Code 2023 day 2 problem: https://adventofcode.com/2023/day/2

#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<numeric>
#include<sstream>
#include<fstream>
#include<chrono>


template<typename T>
T sumAllElements(    std::vector<T> vec,
                     T init)
{
    return accumulate(vec.begin(), vec.end(), init);
}


bool contains(std::string input, std::string substring)
{
    int result = input.find(substring);
    if((result >= 0) && (result <= input.length()))
    {
        return true;
    }
    else if(result == std::string::npos)
    {
        return false;
    }
    return false;
}


std::vector<std::string> separate(  std::string input,
                                    std::string separator = "," )
{
    std::vector<std::string> separatedStrings = {};
    std::string word = "";

    //If we have a single character separator, we can use a more efficient method
    if(separator.length() == 1)
    {
        //Taken from https://stackoverflow.com/a/5757851
        std::istringstream split(input);
        for (std::string each; std::getline(split, each, separator[0]); separatedStrings.push_back(each));
        //Check to see if the last element needs a separator character trimmed off (if there exists a trailing separator character)
        if(!separatedStrings.empty())
        {
            if(separatedStrings.back().back() == separator[0])
            {
                separatedStrings.back().pop_back();
            }
        }
    }
    else
    {
        //Iterate through the input string and piece together the strings we want to separate
        for(int i = 0; i < input.length(); i++)
        {
            word += input[i];
            //Every time we add a character to the word, we check to see if it contains the separator
            if(contains(word, separator))
            {
                //Extract the separator from the word
                word.erase(word.find(separator), separator.length());
                //Push the word into the vector
                separatedStrings.push_back(word);
                //Clear the word
                word.clear();
            }
        }
        //If we had no separators at all, or if the string doesn't end in a separator, we push the remaining
        //word to the vector.
        separatedStrings.push_back(word);
    }
    

    return separatedStrings;
}


/**
 * Remove all leading and trailing whitespace from a string (spaces and tabs), then return it.
 * 
 * Based on: https://stackoverflow.com/a/1798170
 * 
 * Parameter:
 *  std::string str - The string to remove all of the leading and trailing whitespaces from.
 *  std::string whiteSpace - A string containing all of the whitespace characters to trim from the string
 * 
 * Returns:
 *  std::string - str modified by removing all of its leading and trailing whitespaces
*/
std::string trimWhitespace( std::string str,
                            std::string whitespace = " \t"  )
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
    {
        return ""; // no content
    }
        
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


/**
 * Reads and parses the input file for the advent of code day 2 problem.
 * 
 * Parameter:
 *  char * inputFileName - The name of the input file we are loading.
 * 
 * Returns:
 *  unordered_map< int, unordered_map<std::string, int > >
*/
std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > readFile( char * inputFileName)
{
    //Initialize the structure that we'll be storing our parsed data within
    std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData;

    //Open the file
    std::fstream file;
    file.open(inputFileName);
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << inputFileName << "'";
    }

    //File is ready!
    std::string line;
    int gameId;
    //Iterate through all of the lines of hte input file
    while( getline(file,line) )
    {
        //Separation magic to get the game id
        std::vector<std::string> gameIdAndSets = separate(line,":");
        gameId = std::stoi(separate(gameIdAndSets[0]," ")[1]);

        //More separation magic to get the sets
        std::vector<std::string> gameSets =  separate(gameIdAndSets[1],";");

        //Lastly, get the cube quantities for each set with yet more magic
        for(int i=0; i < gameSets.size(); i++)
        {
            std::unordered_map<std::string,int> currSetResult = {};
            std::vector<std::string> quantityStrings = separate(gameSets[i],",");
            for(int c=0; c < quantityStrings.size(); c++)
            {
                std::vector<std::string> cubeData = separate(trimWhitespace(quantityStrings[c])," ");
                currSetResult[cubeData[1]] = std::stoi(cubeData[0]);
            }
            gameData[gameId].push_back(currSetResult);
        }
    }

    file.close();

    return gameData;
}


/**
 * Given the data of the input file and an unordered_map stating the contents of a bag,
 * determine what games are possible to play with the bag, where a game is defined as
 * removing a random number of differently colored cubes from a bag. Once these possible
 * games from the input are determined, output the sum of the game ids that were possible.
 * 
 * Parameters:
 *  std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData - The input file stored as a multilayer data structure.
 *  std::unordered_map<std::string,int> bagContents -   A map describing the contents of the bag, where keys are colors red, green, and blue, and the 
 *                                                      values are the quantity of each color of cube in the bag.
 * 
 * Returns:
 *  void, but prints the sum of the game ids that were possible to play.
*/
void determinePossibleGames(    std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData,
                                std::unordered_map<std::string,int> bagContents )
{
    //For each game, we check to see if any of the cubes pulled are greater than bagContents value for that color of cube
    bool gameIsPossible;
    std::vector<int> possibleGames = {};
    for(const auto & [gameId, resultSet] : gameData)
    {
        gameIsPossible = true;
        for(int i=0; i < resultSet.size(); i++)
        {
            for( const auto & [cubeColor, quantity] : resultSet[i])
            {
                //If the quantity for a cube color in a game is greater than what we have in our bag contents, the game is not possible
                if(quantity > bagContents[cubeColor])
                {
                    gameIsPossible = false;
                    break;
                }
            }
            if(!gameIsPossible)
            {
                break;
            }
        }
        //If a game is possible, we add its id to a vector
        if(gameIsPossible)
        {
            possibleGames.push_back(gameId);
            //std::cout << gameId << std::endl;
        }
    }

    //Sum all of the gameIds and print the result
    //std::cout << "The sum of all possible games: " << sumAllElements(possibleGames,0) << std::endl;
}


/**
 * Given the game data from the input file, determine the fewest number of cubes possible for each color for each game.
 * Once we have the this fewest number of cubes, find the power of the cube set. Output the sum of all of the cube set powers.
 * 
 * Parameter:
 *  std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData - The input file stored as a multilayer data structure.
 * 
 * Returns:
 *  void, but prints the sum of cube powers to the terminal window.
*/
void determineFewestPossibleCubesForGames(  std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData )
{
    int runningSum = 0;
    //Iterate through all of the games
    for(int i = 0; i < gameData.size(); i++)
    {
        //Define the fewest number of cubes possible for the game, or maximum counts drawn
        std::unordered_map<std::string,int> maxCubeCounts = {   {"red",     0},
                                                                {"green",   0},
                                                                {"blue",    0}  };
        //Iterate through each set of the game
        for(int set = 0; set < gameData[i].size(); set++)
        {
            //Iterate through each color of cube in the set and check to see if we can update our maxcube count
            for(const auto & [color, quantity] : gameData[i][set])
            {
                //If we find a quantity that's greater than a color's current maximum, change it
                if(quantity > maxCubeCounts[color])
                {
                    maxCubeCounts[color] = quantity;
                }
            }
        }

        //Find the cube set power and add it to the running sum
        runningSum += (maxCubeCounts["red"] * maxCubeCounts["green"] * maxCubeCounts["blue"]);
    }

    //std::cout << "Cube power sum: " << runningSum << std::endl;
}


int main(   int argc,
            char * argv[]   )
{
    if( argc < 3 )
    {
        std::cout <<    "You must supply 3 arguments to run this program:\n"
                        "0: The program name (day_2)\n"
                        "1: The input file name (input.txt)\n"
                        "2: Part of the problem you want to execute (1 or 2)" << std::endl;
        //return 1; 
    }

    //Store argument 2 as a string
    std::string partToExecute = argv[2];
    //Store argument 3 as a string
    std::string timeFlag = argv[3];

    //Read in file
    std::unordered_map< int, std::vector<std::unordered_map<std::string, int > > > gameData = readFile( argv[1] );

    // if( partToExecute == "1")
    // {
        if(timeFlag == "timed")
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i=0; i < 1000; i++)
            {
                determinePossibleGames( gameData,
                                        {   { "red",    12},
                                            { "green",  13},
                                            { "blue",   14}     });
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average execution time of AoC day 2, part 1 (microseconds): " << ((duration.count() / 1000)) << std::endl;
        }
        else
        {
            determinePossibleGames( gameData,
                                    {   { "red",    12},
                                        { "green",  13},
                                        { "blue",   14}     });
        }
    // }
    // else if( partToExecute == "2")
    // {
        if(timeFlag == "timed")
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i=0; i < 1000; i++)
            {
                determineFewestPossibleCubesForGames( gameData );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 2, part 2 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        else
        {
            determineFewestPossibleCubesForGames( gameData );
        }
    // }
    return 0;
}