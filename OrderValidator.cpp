#include "OrderValidator.h"

// Constructor to initialize OrderValidator
OrderValidator::OrderValidator(const std::map<int, std::vector<int>>& dependencies) : dependencies(dependencies) {}

// Check if the order of filenames is correct
std::pair<bool, std::string> OrderValidator::isOrderCorrect(const std::vector<std::string>& filenames, FilenameMatcher& matcher)
{
    std::vector<int> res = { matcher.filenameToNumber(filenames[0]) };
    int temp = dependencies[matcher.filenameToNumber(filenames[0])].size();
    // Iterate over filenames to check dependencies
    for (int i = 1; i < filenames.size(); ++i)
    {
        if (dependencies[matcher.filenameToNumber(filenames[i])].size() < temp)
        {
            return std::make_pair(false, "The order of filenames is not correct.");
        } else
        {
            temp = dependencies[matcher.filenameToNumber(filenames[i])].size();
        }
        // Check if dependencies are met
        for (const auto& value : dependencies[matcher.filenameToNumber(filenames[i])])
        {
            if (std::find(res.begin(), res.end(), value) == res.end())
            {
                return std::make_pair(false, "Incorrect file processing order.");
            }
        }
        res.push_back(matcher.filenameToNumber(filenames[i]));
    }
    // Check for circular includes
    for (int i = 0; i < dependencies.size(); ++i)
    {
        for (int j = 0; j < dependencies[i].size(); ++j)
        {
            if (std::find(dependencies[j].begin(), dependencies[j].end(), i) != dependencies[j].end())
            {
                return std::make_pair(false, "Circular include detected; file processing is terminated.");
            }
        }
    }
    return std::make_pair(true, "File processing order is correct.");
}