//Advent of Code 2023 day 3
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<numeric>
#include<fstream>
#include<tuple>
#include<chrono>


const std::string NUMERICCHARACTERS = "0123456789";


/**
 * Simple commandline argument parsing class.
 * 
 * Credit: https://stackoverflow.com/a/868894
*/
class InputParser
{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }
        /// @author iain
        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};


/**
 * Given a character c, convert it to a std::string.
 * 
 * Parameter:
 *  char c - The character we'd like to convert to a string.
 * 
 * Returns:
 *  std::string - A string representing the character c.
*/
std::string char_to_string( char c )
{
    std::string s(1, c);
    return s;
}


/**
 * Multiply all elements in the vector and return the result.
 * 
 * Parameters:
 *  std::vector<T> vec - A vector of elements of type T that can be multiplied together.
 *  T init - A number we will multiply our product by. Default is 1.
 * 
 * Returns:
 *  T - Object of type T of the product of all vector elements multiplied together.
*/
template<typename T>
T multiplyAllElements(  std::vector<T> vec,
                        T init = 1 )
{
    return std::accumulate(std::begin(vec), std::end(vec), init, std::multiplies<>());
}


/**
 * The object which stores the data of a single point on the EngineSchematic grid.
*/
class EnginePosition
{
    public:
        /*** Member variables ***/
        char character;
        std::string type;
        int row;
        int col;

        /*** Constructors ***/
        //Default
        EnginePosition()
        {
            character = '.';
            type = "nothing";
            row = 0;
            col = 0;
        }
    
        //Parametric
        EnginePosition( char characterParam,
                        int rowParam,
                        int colParam )
        {
            character = characterParam;
            type = getPositionType(character);
            row = rowParam;
            col = colParam;
        }

        /*** Methods***/
        /**
         * Given a character, determine what type of engine position corresponds
         * to the character.
         * 
         * . returns "nothing"
         * [0123456789] returns "partNumber"
         * Anything other than [.0123456789] returns "symbol"
         * 
         * Parameter:
         *  const char characterParam - The character we are evaluating to find its type.
         * 
         * Returns:
         *  std::string - Indicates the type of EnginePosition for the given characterParam.
        */
        std::string getPositionType( const char characterParam )
        {
            if(characterParam == '.')
            {
                return "nothing";
            }
            else if( NUMERICCHARACTERS.find(characterParam) != std::string::npos )
            {
                return "number";
            }
            else
            {
                return "symbol";
            }
        }

        
    private:

};


/**
 * An object representing the engine schematic for the advent of code 2023 day 3 problem.
 * It is fundamentally a 2D grid where each point on the grid is occupied by nothing (a period .),
 * a symbol (Defined as not a period or a number), or a number (0,1,2,3,4,5,6,7,8,9).
*/
class EngineSchematic
{
    public:
        /*** Member variables ***/
        std::vector< std::vector< EnginePosition > > grid;
        std::vector< std::tuple<int,int> > symbolPositions;
        
        /*** Constructors ***/
        //Default
        EngineSchematic()
        {
            grid = {};
        }

        //Parametric
        EngineSchematic( std::vector< std::vector< EnginePosition > > gridParam)
        {
            grid = gridParam;
            symbolPositions = findSymbolPositions(grid);
        }


        /*** Methods ***/
        /**
         * Given a tuple of integers representing the row and col coordinates of a position on the engine schematic grid,
         * return a vector containing tuple<int,int> coordinates for all adjacent numbers to the passed in coordinates.
         * 
         * Parameter:
         *  tuple<int,int> coordinates - The coordinates on the engine schematic grid we'd like to look around to find all of the adjacent numbers.
         * 
         * Returns:
         *  std::vector< tuple<int,int> > - A vector containing all of the coordinates adjacent to the passed in coordinates which are EnginePositions with type "number"
        */
        std::vector< std::tuple<int,int> > getAdjacentNumbersToPosition( std::tuple<int,int> coordinates )
        {
            //Initialize the vector we'll be returning
            std::vector< std::tuple<int,int> > adjacentNumberCoordinates = {};
            //Make some more readable vars
            int targetRow = std::get<0>(coordinates);
            int targetCol = std::get<1>(coordinates);

            //North
            if(targetRow > 0)
            {
                if(grid[targetRow - 1][targetCol].type == "number")
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow - 1, targetCol));
                }
            }
            //Northeast
            if( (targetRow > 0) && (targetCol < grid[targetRow].size()) )
            {
                if(grid[targetRow - 1][targetCol + 1].type == "number")
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow - 1, targetCol + 1));
                }
            }
            //East
            if( targetCol < grid[targetRow].size() )
            {
                if(grid[targetRow][targetCol + 1].type == "number")
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow, targetCol + 1));
                }
            }
            //Southeast
            if( (targetRow > 0) && (targetCol < grid[targetRow].size()) )
            {
                if( grid[targetRow + 1][targetCol + 1].type == "number" )
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow + 1, targetCol + 1) );
                }
            }
            //South
            if( targetRow > 0 )
            {
                if( grid[targetRow + 1][targetCol].type == "number" )
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow + 1, targetCol) );
                }
            }
            //Southwest
            if( (targetRow > 0 ) && (targetCol > 0) )
            {
                if( grid[targetRow + 1][targetCol - 1].type == "number" )
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow + 1, targetCol - 1) );
                }
            }
            //West
            if( targetCol > 0 )
            {
                if( grid[targetRow][targetCol - 1].type == "number")
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow, targetCol - 1) );
                }
            }
            //Northwest
            if( (targetRow > 0) && (targetCol > 0) )
            {
                if( grid[targetRow - 1][targetCol - 1].type == "number")
                {
                    adjacentNumberCoordinates.push_back( std::make_tuple( targetRow - 1, targetCol - 1) );
                }
            }

            return adjacentNumberCoordinates;
        }


        /**
         * Given the coordinates of a number on the engine schematic grid, look to the left and right of the number to find the whole
         * number, setting each of the positions we find that are of type "number" to type "partNumber". We concatenate all of these 
         * adjacent numbers to the numberCoordinates to form the whole part number, convert it to an integer, and return it.
         * 
         * Paramter:
         *  std::tuple<int,int> numberCoordinates - The coordinates of an engine of type "number" that we have identified to be a part number.
         * 
         * Returns:
         *  int - The integer representation of the part number.
        */
        int findCompletePartNumber( std::tuple<int,int> numberCoordinates )
        {
            //std::cout << "Calling find complete number on row " << std::get<0>(numberCoordinates) << " col " << std::get<1>(numberCoordinates) << std::endl;

            //Start the string representation of our partNumber
            std::string partNumber_str = "";
            partNumber_str += grid[std::get<0>(numberCoordinates)][std::get<1>(numberCoordinates)].character;
            //Set the EnginePosition at numberCoordinates to be a partNumber
            grid[std::get<0>(numberCoordinates)][std::get<1>(numberCoordinates)].type = "partNumber";
            //std::cout << partNumber_str << std::endl;

            //Look to the right of the EnginePosition at numberCoordindates
            for(int col = (std::get<1>(numberCoordinates)+1); col < grid.size(); col++)
            {
                if(grid[std::get<0>(numberCoordinates)][col].type == "number")
                {
                    partNumber_str += grid[std::get<0>(numberCoordinates)][col].character;
                    grid[std::get<0>(numberCoordinates)][col].type = "partNumber";
                }
                else
                {
                    break;
                }
            }
            //Look to the left of the EnginePosition at numberCoordinates
            for(int col = (std::get<1>(numberCoordinates)-1); col >= 0; col--)
            {
                if(grid[std::get<0>(numberCoordinates)][col].type == "number")
                {
                    partNumber_str = grid[std::get<0>(numberCoordinates)][col].character + partNumber_str;
                    grid[std::get<0>(numberCoordinates)][col].type = "partNumber";
                }
                else
                {
                    break;
                }
            }

            //Convert the string representation of our part number into an integer and return it
            //std::cout << "Found partnumber: " << partNumber_str << std::endl;
            return std::stoi(partNumber_str);
        }


    private:

        /*** Methods ***/
        /**
         * Given a grid of engine positions that represents the EngineSchematic, iterate through it to find the
         * the engine positions which have the symbol type.
         * 
         * const std::vector< std::vector< EnginePosition > > grid - The 2D vector representing the grid of the engine schematic.
        */
        std::vector< std::tuple<int,int> > findSymbolPositions( const std::vector< std::vector< EnginePosition > > gridParam )
        {
            //Initialize the vector that we'll be returning
            std::vector< std::tuple<int,int> >foundSymbolPositions = {};

            //Iterate through the rows of the grid
            for(int row = 0; row < gridParam.size(); row++)
            {
                //Iterate through the columns of the grid
                for(int col = 0; col < gridParam[row].size(); col++)
                {
                    if(gridParam[row][col].type == "symbol")
                    {
                        //std::cout << grid[row][col].character << std::endl;
                        foundSymbolPositions.push_back( std::make_tuple(row, col) );
                    }
                }
            }

            return foundSymbolPositions;
        }
        
};


/**
 * Given an engine schematic object, find the sum of all of the part numbers in the schematic. Part numbers are defined 
 * as any sequence of numbers that are adjacent to a non-period symbol in the schematic's grid.
 * 
 * Parameter:
 *  EngineSchematic engineSchematic - The engineSchematic object we are working with to find the sum of its part numbers.
 *  bool timed - Indicates whether the function is being timed for execution speed or not.
 * 
 * Returns:
 *  void, but prints the sum of all part numbers using std::cout.
*/
void findSumOfPartNumbers(EngineSchematic engineSchematic, bool timed )
{
    //Begin a running sum of all of the part numbers
    int runningSum = 0;
    //Iterate through all of the known symbol positions
    for(int i = 0; i < engineSchematic.symbolPositions.size(); i++)
    {
        //Get all of the adjacent engine positions of type "number" to the symbol
        std::vector< std::tuple<int,int> > adjacentNumberPositions = engineSchematic.getAdjacentNumbersToPosition(engineSchematic.symbolPositions[i]);
        
        //Iterate through all of these number adjacent engine positions
        for(int k = 0; k < adjacentNumberPositions.size(); k++)
        {
            //Before we add compare the part numbers, make sure we haven't already compared them
            if(engineSchematic.grid[std::get<0>(adjacentNumberPositions[k])][std::get<1>(adjacentNumberPositions[k])].type == "number")
            {    
                //Look to the left and right of the number to find the complete number
                int partNumber = engineSchematic.findCompletePartNumber(adjacentNumberPositions[k]);

                //Add the part number to running sum
                runningSum += partNumber;
            }
        }
    }
    //Print the sum of part numbers
    // if(!timed)
    // {
    //     std::cout << "Sum of all part numbers: " << runningSum << std::endl;
    // }
}


/**
 * Day 3 part 2 of Advent of Code
 * 
*/
int findSumOfGearRatios( EngineSchematic engineSchematic )
{
    //Find all of the gears. Do this by searching through our symbols and finding the * adjacent to two partNumbers
    //Initialize the sum of gear ratios
    int sumOfGearRatios = 0;
    //Iterate through all of the symbols
    for(int i = 0; i < engineSchematic.symbolPositions.size(); i++)
    {
        EnginePosition currPosition = engineSchematic.grid[std::get<0>(engineSchematic.symbolPositions[i])][std::get<1>(engineSchematic.symbolPositions[i])];
        //Check to see if the symbol is a *
        if(currPosition.character == '*')
        {
            //If the symbol is *, check to see how many adjacent part numbers there are. If there are 2, then it is a gear.
            std::vector<int> adjacentPartNumbers = {};
            //To do this, we'll start with getting all adjacent numbers
            std::vector< std::tuple<int,int> > adjacentNumberPositions = engineSchematic.getAdjacentNumbersToPosition(engineSchematic.symbolPositions[i]);
            //Iterate through all of these number adjacent engine positions
            for(int k = 0; k < adjacentNumberPositions.size(); k++)
            {
                //Before we add compare the part numbers, make sure we haven't already compared them
                if(engineSchematic.grid[std::get<0>(adjacentNumberPositions[k])][std::get<1>(adjacentNumberPositions[k])].type == "number")
                {    
                    //Look to the left and right of the number to find the complete number
                    adjacentPartNumbers.push_back( engineSchematic.findCompletePartNumber(adjacentNumberPositions[k]) );
                }
            }
            //If we have exactly two part numbers, multiply them together to find the gear ratio
            if(adjacentPartNumbers.size() == 2)
            {
                sumOfGearRatios += multiplyAllElements(adjacentPartNumbers);
            }
            adjacentPartNumbers.clear();
        }
    }
    //Return the sum of gear ratios
    return sumOfGearRatios;
}


/**
 * Given the input file for day 3 of Advent of Code 2023, return an engine schematic object
 * that we can process for the problem.
 * 
 * Parameter:
 *  std::string inputFileName - The name of the input file.
 * 
 * Returns:
 *  EngineSchematic - The engine schematic object we will use for the problem.
*/
EngineSchematic readInput( std::string inputFileName )
{
    //Initialize the vector that we will construct the EngineSchematic with
    std::vector< std::vector< EnginePosition > > grid = {}; 

    //Open the input file
    std::fstream file;
    file.open(inputFileName);
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << inputFileName << "'";
    }

    //File is ready to go!
    std::string line;
    std::vector<EnginePosition> row = {};
    int rowNum = 0;
    //Iterate through the file and add each line as a row of the engine schematic
    while( getline( file, line ) )
    {
        for(int p=0; p < line.length(); p++)
        {
            row.push_back(EnginePosition(line[p], rowNum, p));
        }
        grid.push_back(row);
        row.clear();
        rowNum++;
    }

    //Close the file
    file.close();

    //Construct the EngineSchematic and return it
    EngineSchematic engineSchematic = EngineSchematic(grid);
    return engineSchematic;
}


int main(   int argc, char * argv[]   )
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
    EngineSchematic engineSchematic = readInput( inputFileName );

    if(partToExecute == "1")
    {
        //Find the average time of the execution
        if(inputParser.cmdOptionExists("-t"))
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < 1000; i++)
            {
                findSumOfPartNumbers(engineSchematic, true);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 3, part 1 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        //Execute normally
        else
        {
            findSumOfPartNumbers(engineSchematic, false);
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
                findSumOfGearRatios(engineSchematic);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 3, part 2 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        //Execute normally
        else
        {
            int answer = findSumOfGearRatios( engineSchematic );
            std::cout << "Sum of gear ratios: " << answer << std::endl;
        }
    }

    return 0;
}