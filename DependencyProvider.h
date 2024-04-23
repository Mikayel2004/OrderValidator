#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "FilenameMatcher.h"

class DependencyProvider
{
    std::map<int, std::vector<int>> dependencies;

public:
    // Constructor to initialize DependencyProvider
    DependencyProvider(const std::string& directoryPath, FilenameMatcher& matcher, std::vector<std::string> filenames);

    // Provide the dependencies map
    std::map<int, std::vector<int>> provideDependencies();
};
