#pragma once

#include <vector>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include "FilenameMatcher.h"

class OrderValidator
{
    std::map<int, std::vector<int>> m_dependencies;

public:
    // Constructor to initialize OrderValidator
    OrderValidator(const std::map<int, std::vector<int>>& dependencies);

    // Check if the order of filenames is correct
    std::pair<bool, std::string> isOrderCorrect(const std::vector<std::string>& filenames, FilenameMatcher& matcher);
    bool isCyclic(int v, std::vector<bool>& visited, std::vector<bool>& dfsVisited);
};