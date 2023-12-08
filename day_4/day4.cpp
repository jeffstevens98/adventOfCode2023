//Advent of Code 2023, Day 4: Scratchcards
//@author Jeff Stevens

#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
#include<chrono>
#include<unordered_map>
#include "InputParser.h"
#include "stevensStringLib.h"


/**
 * Predicate for eraseNonNumericChars. Determines if a character is non-numeric (true) or numeric (false)
*/
bool isNonNumeric( char c )
{
    std::unordered_map<char,bool> numericCharacters = { {'0',   0},
                                                        {'1',   0},
                                                        {'2',   0},
                                                        {'3',   0},
                                                        {'4',   0},
                                                        {'5',   0},
                                                        {'6',   0},
                                                        {'7',   0},
                                                        {'8',   0},
                                                        {'9',   0}  };
    
    if(numericCharacters.contains(c))
    {
        return false;
    }
    else
    {
        return true;
    }
}


/**
 * Erases all non-numeric characters from a string and returns it.
 * 
 * Parameter:
 *  std::string str - The string to release all non-numeric chars from.
 * 
 * Returns:
 *  The string parameter we passed in, but all of its non-numeric characters removed from it.
*/
std::string eraseNonNumericChars(   std::string str )
{
    //std::cout << "str befor eraseNonNumericChars:" << str << std::endl;
    str.erase(std::remove_if(str.begin(), str.end(), isNonNumeric), str.end());
    //std::cout << "str after eraseNonNumericChars: " << str << std::endl; 
    return str;
}


/**
 * Representation of the ScratchCard data for the day 4 problem.
*/
class ScratchCard
{
    public:
        /*** Variables ***/
        int id; //The id number of the scratchcard
        std::vector<int> candidateNumbers; //The numbers scratched off on the card.
        std::unordered_map<int,int> winningNumbers; //The numbers that will earn points if seen in the candidate numbers. Keys are the winning numbers and values are zero.
        int quantity; //The number of copies we have of this card


        /*** Constructors ***/
        //Default constructor
        ScratchCard()
        {
            id = 0;
            candidateNumbers = {};
            winningNumbers = {};
            quantity = 1;
        }


        //Parametric constructor
        ScratchCard(    int idParam,
                        std::vector<int> candidateNumbersParam,
                        std::unordered_map<int,int> winningNumbersParam  )
        {
            id = idParam;
            candidateNumbers = candidateNumbersParam;
            winningNumbers = winningNumbersParam;
            quantity = 1;
        }


        /*** Methods ***/
        /**
         * Calculate the score of the scratchcard and return it as an integer.
         * The first match you get on a card gives one point, and all further matches change the point value of the card to double of 
         * its previous value.
        */
        int getScore()
        {
            int matches = getNumberOfMatches();
            return (int)pow( 2, (matches-1) );
        }


        /**
         * Returns the integer number of candidate numbers that match with winning numbers.
        */
        int getNumberOfMatches()
        {
            int matches = 0;
            //Iterate through all of the candidate numbers
            for(int i = 0; i < candidateNumbers.size(); i++)
            {
                //Check to see if the candidate number is a winning number
                if(winningNumbers.contains(candidateNumbers[i]))
                {
                    matches++;
                }
            }
            return matches;
        }


    private:
};


/**
 * Part 1 of the day 4 problem:
 * Given a bunch of ScratchCard objects as input, determine the sum of all of their scores.
 * 
 * Parameter:
 *  std::vector< ScratchCard > scratchCards - The collection of scratchcards which we are summing the scores of.
 * 
 * Returns:
 *  int - The sum of all of the scratchcard scores.
*/
int findTotalPointsOfScratchCards(  std::vector< ScratchCard > scratchCards )
{
    //Initialize our sum
    int sumOfPoints = 0;

    //For each scratchcard, add its points to the sum
    for(int i = 0; i < scratchCards.size(); i++)
    {
        sumOfPoints += scratchCards[i].getScore();
    }
    
    return sumOfPoints;
}


/**
 * Part 2 of the day 4 problem:
 * Given a bunch of ScratchCard objects as input, where each ScratchCard earns you copies of the next X ScratchCards for having X winning numbers,
 * determine how many total scratchcards you have in total by summing the original quantity of cards with all of the copies you've won.
 * 
 * Parameter:
 *  std::vector< ScratchCard > scratchCards - A vector containing all of the ScratchCards.
 * 
 * Returns:
 *  int - The total number of all scratchcards after we've figured out how many copies we've won.
*/
int findTotalNumberOfScratchCards(  std::vector<ScratchCard> scratchCards )
{
    //Initialize the total number of scratchcards
    int cardTotal = 0;

    //std::cout << "Total number of scratchcards BEFORE reading them: " << scratchCards.size() << std::endl;

    //For each scratchcard, add its quantity to the cardTotal and find out how many copies of future cards it wins
    for(int i = 0; i < scratchCards.size(); i++)
    {
        cardTotal += scratchCards[i].quantity;
        int nextCardsWon = scratchCards[i].getNumberOfMatches();
        int copiesOfNextCards = scratchCards[i].quantity;
        int c = i + 1;
        while( nextCardsWon > 0)
        {
            if(c < scratchCards.size())
            {
                scratchCards[c].quantity += copiesOfNextCards;  
            }
            c++;
            nextCardsWon--;
        }
    }

    return cardTotal;
}


/**
 * Reads the input file in and creates a std::vector of ScratchCard objects we can work with.
 * 
 * Parameter:
 *  std::string inputFileName - The std::string name of the file that will be used to read scratchcard data into objects.
 * 
 * Returns:
 *  std::vector< ScratchCard > - A std::vector container of ScratchCard objects that we'll be able to use for this problem.
*/
std::vector< ScratchCard > readScratchCards( std::string inputFileName )
{
    //Open the file
    std::fstream file(inputFileName);
    std::cout << "Opening scratchcard file...\n";
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << inputFileName << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    //File is ready!
    std::cout << "Succesfully opened scratchcard file!\n";

    //We'll be using a string lib to slice up the data
    stevensStringLib stringLib = stevensStringLib();
    //We also need a place to store our scrachcards as we read them:
    std::vector< ScratchCard > scratchCards = {};
    //Go through each line of the input file and read the scrachcard data
    std::string line;
    while( getline( file, line ) )
    {
        //Separate the card id from the numbers
        std::vector<std::string> cardIdAndNumbers = stringLib.separate(line, ":");
        int cardId = std::stoi(eraseNonNumericChars(cardIdAndNumbers[0]));
        //std::cout << "stoi works for cardId" << std::endl;
        //Separate the winning numbers from the candidate numbers
        std::vector<std::string> candidateAndWinningNumbers = stringLib.separate(cardIdAndNumbers[1], "|");
        //Load the candidate numbers into a vector of integers
        std::vector<std::string> candidateNumbers_str = stringLib.separate( stringLib.trimWhitespace(candidateAndWinningNumbers[1]), " ");
        std::vector<int> candidateNumbers = {};
        for(int i = 0; i < candidateNumbers_str.size(); i++)
        {
            if(candidateNumbers_str[i] != "")
            {
                candidateNumbers.push_back(std::stoi(candidateNumbers_str[i]));
            }
        }
        //Load the winning numbers into an unordered map
        std::vector<std::string> winningNumbers_str = stringLib.separate( stringLib.trimWhitespace(candidateAndWinningNumbers[0]), " ");
        std::unordered_map<int,int> winningNumbers;
        for(int i = 0; i < winningNumbers_str.size(); i++)
        {
            if(winningNumbers_str[i] != "")
            {
                winningNumbers[std::stoi(winningNumbers_str[i])] = 0;
            }
        }
        //Construct the scratchcard
        ScratchCard card = ScratchCard( cardId, candidateNumbers, winningNumbers);
        //Add it to the vector
        scratchCards.push_back(card);
        //std::cout << "scratchcard pushed back!" << std::endl;
    }
    std::cout << "Read all scratchcards!" << std::endl;

    //Close the file
    file.close();

    return scratchCards;
}


int main( int argc, char * argv[])
{
    InputParser inputParser =  InputParser(argc, argv);

    std::string inputFileName = inputParser.getCmdOption("-f");
    std::string partToExecute = inputParser.getCmdOption("-p");
    bool commandLineArgsError = false;
    
    if(inputFileName.empty())
    {
        std::cout << "You must supply -f <INPUT FILENAME> as a commandline argument" << std::endl;
        commandLineArgsError = true;
    }
    if(partToExecute.empty())
    {
        std::cout << "You must supply -p <PART TO EXECUTE> (1 or 2) as a commandline argument" << std::endl;
        commandLineArgsError = true;
    }
    if(commandLineArgsError)
    {
        return 1;
    }

    //Read in the file
    std::vector< ScratchCard > scratchCards  = readScratchCards( inputFileName );

    if(partToExecute == "1")
    {
        //Find the average time of the execution
        if(inputParser.cmdOptionExists("-t"))
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < 1000; i++)
            {
                findTotalPointsOfScratchCards( scratchCards );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 4, part 1 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        //Execute normally
        else
        {
        int totalPoints = findTotalPointsOfScratchCards( scratchCards );
        std::cout << "Total points of all scratchcards: " << totalPoints << std::endl;
        }
    }
    if(partToExecute == "2")
    {
        //Find the average time of execution
        if(inputParser.cmdOptionExists("-t"))
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < 1000; i++)
            {
                findTotalNumberOfScratchCards( scratchCards );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 4, part 2 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        //Execute normally
        else
        {
            int cardCount = findTotalNumberOfScratchCards( scratchCards );
            std::cout << "Total number of scratchcards: " << cardCount << std::endl;
        }
    }

    return 0;
}