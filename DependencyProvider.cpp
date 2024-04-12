#include "DependencyProvider.h"

DependencyProvider::DependencyProvider(const std::string& directoryPath, FilenameMatcher& matcher, std::vector<std::string> filenames)
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

std::map<int, std::vector<int>> DependencyProvider::provideDependencies()
{
    return dependencies;
}