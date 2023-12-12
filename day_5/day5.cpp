//Advent of Code 2023, Day 5
//@author Jeff Stevens


#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "InputParser.h"
#include "stevensStringLib.h"


stevensStringLib stringLib = stevensStringLib(); 


/**
 * Given a std::vector<std::string>, returns a std::vector<int>, where all std::strings in the original vector are converted to integers
 * that they represent.
 * 
 * May throw error with std::stoi if the parameter vector contains an string that does not properly represent an integer.
 * 
 * Taken from:
 * https://stackoverflow.com/a/15619863/16511184
 * 
 * Parameter:
 *  std::vector<std::string> inputVec - A vector of std::strings, where each string represents an integer, which we wish to convert into a std::vector<int>.
 * 
 * Returns:
 *  std::vector<int> - Our parameter vector, but all of its elements converts to the ints.
*/
std::vector<int> vecOfStrings_to_vecOfInts( std::vector<std::string> inputVec)
{
    std::vector<int> outputVec = {};

    std::transform(inputVec.begin(), inputVec.end(), std::back_inserter(outputVec),
               [](const std::string& str) { return std::stoi(str); });

    return outputVec;
}


std::vector<long long int> vecOfStrings_to_vecOfLongLongInts( std::vector<std::string> inputVec)
{
    std::vector<long long int> outputVec = {};

    std::transform(inputVec.begin(), inputVec.end(), std::back_inserter(outputVec),
               [](const std::string& str) { return std::stoll(str); });

    return outputVec;
}


/**
 * Given a vector of type T, vec, return the minimum element contained within it.
 * 
 * Parameter:
 *  std::vector<T> vec - A vector with elements of type T.
 * 
 * Returns:
 *  T - The minimum element of vec
*/
template<typename T>
T find_min( const std::vector<T> vec)
{
    //Throw an error if the vector is empty
    if(vec.empty())
    {
        std::cerr << "stevensVectorLib Error: find_min_int(): Parameter must be a vector with size greater than 0" << std::endl;
        exit(EXIT_FAILURE);
    }

    return *min_element(std::begin(vec), std::end(vec));
}


/**
 * TOOD: How to make this only work for numerical types?
 * 
 * Given a lower bound and upper bound of some numerical type, check to see if a variable in within the range specified by the upper and lower bounds.
 * If it is, return true. Otherwise, return false.
 * 
 * Parameters:
 *  const T checkValue - The value which we are checking to see if it falls between the bounds
 *  const T lowerBound - The smaller value we are checking to see if our value is greater than/greater than or equal to.
 *  const T upperBound - The bigger  value we are checking to see if our value is less than/less than or equal to.
 *  std::string boundSetting - The way which we are treating the edge cases of our range checking. Possible values are:
 *                                  "exclusive" - Bounds are checked using > and < signs
 *                                  "inclusive" - Bounds are checked using >= and <= signs
 * 
 * Returns:
 *  bool - True if the checkValue falls within the bounds, False otherwise.
*/
template<typename T>
bool in_range(  const T checkValue,
                const T lowerBound,
                const T upperBound,
                std::string boundSetting = "exclusive"  )
{
    if(boundSetting == "exclusive")
    {
        if((checkValue > lowerBound) && (checkValue < upperBound))
        {
            return true;
        }
    }
    else if (boundSetting == "inclusive")
    {
        if((checkValue >= lowerBound) && (checkValue <= upperBound))
        {
            return true;
        }
    }
    else
    {
        std::cerr << "StevensMathLib Error: in_range: Could not recognize the boundSetting parameter '" << boundSetting << "'." << std::endl;
    }

    return false;
}


/**
 * Used during reading in the file for advent of code 2023 day 5. Stores data about an individual line from the input file:
 * -Source range start
 * -Destination range start
 * -Range length
 * 
*/
class MappingRange
{
    public:
        /*** Variables ***/
        long long int sourceRangeStart;
        long long int destinationRangeStart;
        long long int rangeLength; 

        /*** Constructors ***/
        //Default
        MappingRange()
        {
        }


        //Parametric
        MappingRange(   long long int destinationRangeStartParam,
                        long long int sourceRangeStartParam,
                        long long int rangeLengthParam    )
        {
            sourceRangeStart = sourceRangeStartParam;
            destinationRangeStart = destinationRangeStartParam;
            rangeLength = rangeLengthParam;
        }
    private:
};


/**
 * Class representing the mappings of seed_to_soil, soil_to_fertilizer, fertilizer_to_water, water_to_light, light_to_temperature, temperature_to_humidity,
 * and humidity_to_location for all of the seeds on island island!
*/
class Almanac
{
    public:
        /*** Variables ***/
        std::vector<MappingRange> seed_to_soil;
        std::vector<MappingRange> soil_to_fertilizer;
        std::vector<MappingRange> fertilizer_to_water;
        std::vector<MappingRange> water_to_light;
        std::vector<MappingRange> light_to_temperature;
        std::vector<MappingRange> temperature_to_humidity;
        std::vector<MappingRange> humidity_to_location;

        /*** Constructors ***/
        //Default
        Almanac()
        {
        }


        //Parametric
        Almanac(    std::vector<MappingRange> seed_to_soil_param,
                    std::vector<MappingRange> soil_to_fertilizer_param,
                    std::vector<MappingRange> fertilizer_to_water_param,
                    std::vector<MappingRange> water_to_light_param,
                    std::vector<MappingRange> light_to_temperature_param,
                    std::vector<MappingRange> temperature_to_humidity_param,
                    std::vector<MappingRange> humidity_to_location_param    )
        {
            seed_to_soil = seed_to_soil_param;
            soil_to_fertilizer = soil_to_fertilizer_param;
            fertilizer_to_water = fertilizer_to_water_param;
            water_to_light = water_to_light_param;
            light_to_temperature = light_to_temperature_param;
            temperature_to_humidity = temperature_to_humidity_param;
            humidity_to_location = humidity_to_location_param;
        }


        /*** Methods ***/
    private:
};


/**
 * Given a vector of mapping range data, create a mapping of data from the source ranges to the destination ranges.
 * Return it as represented by a std::map<int,int>
 * 
 * Parameter:
 *  std::vector<MappingRange> mappingData - A vector of MappingRange objects describing which source ranges are mapped to which destination ranges.
 * 
 * Returns:
 *  std::map<int,int> - A map object describing the one-to-one relationships between a source number and a destination number from the mappingData parameter.
*/
std::map<long long int, long long int> createMapping( std::vector<MappingRange> mappingData)
{
    std::map<long long int, long long int> mapping = {};

    //Now that our default mapping exists, we iterate through our mappingData and assign specific mappings
    for(int i = 0; i < mappingData.size(); i++)
    {
        for(int k = 0; k < mappingData[i].rangeLength; k++)
        {
            mapping[mappingData[i].sourceRangeStart + k] = mappingData[i].destinationRangeStart + k;
        }
    }

    return mapping;
}


/**
 * Given a source value, find its mapping from a vector of mapping ranges from an almanac object.
 * 
 * Parameters:
 *  long long int source - The number we wish to find the mapping of within the almanacMapping.
 *  std::vector<MappingRange> almanacMapping - A vector of mapping ranges that contains the destination number for our source number.
 * 
 * Returns:
 *  long long int - The destination number that our source number is mapped to.
*/
long long int getAlmanacMapping(    long long int source, 
                                    std::vector<MappingRange> almanacMapping)
{
    //For every mapping range, check to see if the source is within the mapping range
    bool foundMapping = false;
    long long int destination;
    for(int i = 0; i < almanacMapping.size(); i++)
    {
        if( in_range(source, almanacMapping[i].sourceRangeStart, (almanacMapping[i].sourceRangeStart + almanacMapping[i].rangeLength - 1), "inclusive") )
        {
            //If the source is found within the mapping range, we find its destination
            destination = almanacMapping[i].destinationRangeStart + (source - almanacMapping[i].sourceRangeStart);
            //std::cout << "Calculating almanac mapping:" << source << " - " << almanacMapping[i].sourceRangeStart << " + " << almanacMapping[i].destinationRangeStart << " = " << destination << std::endl;
            foundMapping = true;
        }
    }
    if(!foundMapping)
    {
        //If we did not find the mapping for our source in our defined mapping ranges, then the source's destination is equal to itself.
        destination = source;
    }

    return destination;
}


/**
 * Given the input for advent of code 2023 day 5, read it into the program. We'll store it as a tuple
 * of a std::vector<int> and an Almanac.
*/
std::tuple<std::vector<long long int>, Almanac> readInput( std::string inputFileName )
{
    //Open the file
    std::fstream file(inputFileName);
    std::cout << "Opening seeds and almanac file...\n";
    if(!file.is_open())
    {
        std::cout << "There was a problem and we were unable to open a file with the name:'" << inputFileName << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    //File is ready!
    std::cout << "Succesfully opened file!\n";

    //Now we'll iterate through the file
    std::string line;
    int activity = 0; //Looking to identify the next activity
    //First, we'll read in the seeds from the first line
    std::vector<long long int> seeds = {};
    //After that, we'll start populating the mapping vairbles in the Almanac
    std::vector<MappingRange> mappingData = {};
    Almanac almanac = Almanac();
    while( getline(file, line) )
    {
        switch(activity)
        {
            //Looking to identify the next activity
            case 0:
                //Check if the line has any content
                if(!line.empty())
                {
                    //Find how the line is labeled
                    std::vector<std::string> separatedLabelLine = stringLib.separate(line, ":");
                    std::string label = separatedLabelLine[0];
                    if(label == "seeds")
                    {
                        //Read the seeds in from  this line
                        seeds = vecOfStrings_to_vecOfLongLongInts( stringLib.separate( stringLib.trimWhitespace(separatedLabelLine[1]), " ") );
                    }
                    else if(label == "seed-to-soil map")
                    {
                        //Switch to processing seed-to-soil mapping
                        activity = 1;
                    }
                    else if(label == "soil-to-fertilizer map")
                    {
                        //Switch to processing soil-to-fertilizer mapping
                        activity = 2;
                    }
                    else if(label == "fertilizer-to-water map")
                    {
                        //Switch to processing fertilizer-to-water mapping
                        activity = 3;
                    }
                    else if(label == "water-to-light map")
                    {
                        //Switch to processing water-to-light mapping
                        activity = 4;
                    }
                    else if(label == "light-to-temperature map")
                    {
                        //Switch to processing light-to-temperature mappping
                        activity = 5;
                    }
                    else if(label == "temperature-to-humidity map")
                    {
                        //Switch to processing temperature-to-humidity mapping
                        activity = 6;
                    }
                    else if(label == "humidity-to-location map")
                    {
                        //Swtich to processing humidity-to-location mapping
                        activity = 7;
                    }
                    mappingData.clear();
                }
                else
                {
                    //No content in this line. Go to the next one and look there.
                    continue;
                }
                break;
            //Create seed-to-soil mapping
            case 1:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.seed_to_soil = mappingData;
                    activity = 0;
                    //std::cout << "Size of seed to soil: " << almanac.seed_to_soil.size() << std::endl;
                }
                break;
            //Create soil-to-fertilizer mapping
            case 2:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.soil_to_fertilizer = mappingData;
                    activity = 0;
                    //std::cout << "Size of soil to fertilizer: " << almanac.soil_to_fertilizer.size() << std::endl;
                }
                break;
            //Create fertilizer-to-water mapping
            case 3:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.fertilizer_to_water = mappingData;
                    activity = 0;
                    //std::cout << "Size of fertilizer to water: " << almanac.fertilizer_to_wcater.size() << std::endl;
                }
                break;
            //Create water-to-light mapping
            case 4:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.water_to_light = mappingData;
                    activity = 0;
                }
                break;
            //Create light-to-temperature mapping
            case 5:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.light_to_temperature = mappingData;
                    activity = 0;
                }
                break;
            //Create temperature-to-humidity mapping
            case 6:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                else
                {
                    almanac.temperature_to_humidity = mappingData;
                    activity = 0;
                }
                break;
            //Create humidity-to-location mapping
            case 7:
                //If the line contains content
                if(!line.empty())
                {
                    //Store the data from the line in a mapping range object
                    std::vector<long long int> lineData = vecOfStrings_to_vecOfLongLongInts(stringLib.separate(line, " "));
                    mappingData.push_back(MappingRange(lineData[0], lineData[1], lineData[2]));
                }
                // else
                // {
                //     
                //     activity = 0;
                // }
                break;
            default:
                //std::cout << "Hitting the default case" << std::endl;
                break;
        }
    }
    almanac.humidity_to_location = mappingData;

    return std::make_tuple(seeds, almanac);
}


/**
 * Given a vector of seeds, run through the convoluted process of looking through seed mappings in the almanac to 
 * find the closest (lowest) location number that any one of the seeds are mapped to, then return the lowest location
 * number.
 * 
 * Parameters:
 *  std::vector<int> seeds - A vector of integers describing the id numbers of seeds we are looking up the location numbers of.
 *  Almanac almanac - The agricultural almanac for island island, describing how different agricultural materials are mapped to eachother.
 * 
 * Returns:
 *  int - The lowest location number that is mapped to one of the elements from the seeds vector.
*/
int findClosestSeedLocation(    std::vector<long long int> seeds,
                                Almanac almanac )
{
    std::vector<long long int> locations = {};

    long long int soil, fertilizer, water, light, temperature, humidity, location;

    //For each of our seeds, find its location
    for(int i = 0; i < seeds.size(); i++)
    {
        soil = getAlmanacMapping(seeds[i], almanac.seed_to_soil);
        //std::cout << "Seed " << seeds[i] << "\n-> Soil " << soil << std::endl;
        fertilizer = getAlmanacMapping(soil, almanac.soil_to_fertilizer);
        //std::cout << "-> Fertilizer " << fertilizer << std::endl;
        water = getAlmanacMapping(fertilizer, almanac.fertilizer_to_water);
        //std::cout << "-> Water " << water << std::endl; 
        light = getAlmanacMapping(water, almanac.water_to_light);
        //std::cout << "-> Light " << light << std::endl;
        temperature = getAlmanacMapping(light, almanac.light_to_temperature);
        //std::cout << "-> Temperature  " << temperature << std::endl;
        humidity = getAlmanacMapping(temperature, almanac.temperature_to_humidity);
        //std::cout << "-> Humidity " << humidity << std::endl;
        location = getAlmanacMapping(humidity, almanac.humidity_to_location);
        //std::cout << "-> LOCATION OF SEED " << seeds[i] << ": " << location << "\n" << std::endl;
        locations.push_back(location);
    }

    return find_min(locations);
}


int main( int argc, char * argv[] )
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


    if(partToExecute == "1")
    {
        //Read in the file
        std::tuple< std::vector<long long int>, Almanac > seedsAndAlmanac = readInput( inputFileName );
        std::vector<long long int> seeds = get<0>(seedsAndAlmanac);
        Almanac almanac = get<1>(seedsAndAlmanac);

        //Find the average time of the execution
        if(inputParser.cmdOptionExists("-t"))
        {
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < 1000; i++)
            {
                findClosestSeedLocation( seeds, almanac );
            }
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "The average exeuction time of AoC day 5, part 1 (microseconds): " << ((duration.count() /1000)) << std::endl;
        }
        //Execute normally
        else
        {
            int answer = findClosestSeedLocation(seeds, almanac);
            std::cout << "Closest (lowest) seed location: " << answer << std::endl;
        }
    }
    if(partToExecute == "2")
    {
        //Read in the file
        std::tuple<, almanac> readInput_p2( inputFileName );

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