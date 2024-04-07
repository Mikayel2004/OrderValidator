#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

class FilenameMatcher
{
private:
    std::unordered_map<std::string, int> filenameToNumberMap;
    std::vector<std::string> numberToFilenameMap;

public:
    // Constructor to initialize FilenameMatcher
    FilenameMatcher(const std::string& directoryPath)
    {
        int number = 0;
        // Iterate over files in the specified directory
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".h")
            {
                std::string filename = entry.path().filename().string();
                // Check if filename contains "header"
                if (filename.find("header") != std::string::npos)
                {
                    filenameToNumberMap[filename] = number;
                    numberToFilenameMap.push_back(filename);
                    number++;
                }
            }
        }
    }

    // Convert filename to its corresponding number
    int filenameToNumber(const std::string& filename)
    {
        return filenameToNumberMap[filename];
    }

    // Convert number to its corresponding filename
    std::string numberToFilename(int number)
    {
        return numberToFilenameMap[number];
    }
};

class DependencyProvider
{
    std::map<int, std::vector<int>> dependencies;

public:
    // Constructor to initialize DependencyProvider
    DependencyProvider(const std::string& directoryPath, FilenameMatcher& matcher, std::vector<std::string> filenames)
    {
        // Iterate over filenames provided
        for (const auto& filename : filenames)
        {
            std::ifstream file(directoryPath + "/" + filename);
            if (!file.is_open())
            {
                throw std::runtime_error("Could not open file: " + filename);
            }
            std::string line;
            bool flag = false;
            // Read file line by line
            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string token;
                while (iss >> token)
                {
                    if (token == "#include")
                    {
                        flag = true;
                        std::string headerName;
                        iss >> headerName;
                        if (headerName.find("header") != std::string::npos)
                        {
                            std::string dependency = headerName.substr(1, headerName.size() - 2);
                            int fileNumber = matcher.filenameToNumber(filename);
                            int dependencyNumber = matcher.filenameToNumber(dependency);
                            dependencies[fileNumber].push_back(dependencyNumber);
                        }
                    }
                }
                if (!flag)
                {
                    int fileNumber = matcher.filenameToNumber(filename);
                    dependencies[fileNumber] = {};
                }
            }
            file.close();
        }
        // Sort dependencies for each file
        for (auto& pair : dependencies)
        {
            std::sort(pair.second.begin(), pair.second.end());
        }
    }
    // Provide the dependencies map
    std::map<int, std::vector<int>> provideDependencies()
    {
        return dependencies;
    }
};

class OrderValidator
{
    std::map<int, std::vector<int>> dependencies;

public:
    // Constructor to initialize OrderValidator
    OrderValidator(const std::map<int, std::vector<int>>& dependencies) : dependencies(dependencies) {}

    // Check if the order of filenames is correct
    std::pair<bool, std::string> isOrderCorrect(const std::vector<std::string>& filenames, FilenameMatcher& matcher)
    {
        std::vector<int> res = {matcher.filenameToNumber(filenames[0])};
        int temp = dependencies[matcher.filenameToNumber(filenames[0])].size();
        // Iterate over filenames to check dependencies
        for (int i = 1; i < filenames.size(); ++i)
        {
            if (dependencies[matcher.filenameToNumber(filenames[i])].size() < temp)
            {
                return std::make_pair(false, "The order of filenames is not correct.");
            }
            else
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
};

class OrderValidatingEngine
{
public:
    // Execution of the methods above
    void execute(std::istream& in, std::ostream& out)
    {
        std::string directoryPath;
        std::getline(in, directoryPath);

        std::vector<std::string> filenames;
        std::string filename;
        // Read filenames
        while (std::getline(in, filename))
        {
            filenames.push_back(filename);
        }

        FilenameMatcher matcher(directoryPath);
        DependencyProvider provider(directoryPath, matcher, filenames);

        std::map<int, std::vector<int>> dependencies = provider.provideDependencies();
        OrderValidator validator(dependencies);

        auto result = validator.isOrderCorrect(filenames, matcher);
        // Output result
        if (result.first)
        {
            out << result.second << std::endl;
        }
        else
        {
            out << result.second << std::endl;
        }
    }
};

int main()
{
    OrderValidatingEngine orderValidatingEngine;
    orderValidatingEngine.execute(std::cin, std::cout);
    return 0;
}
