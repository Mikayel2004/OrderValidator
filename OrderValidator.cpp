#include "OrderValidator.h"

// Constructor to initialize OrderValidator
OrderValidator::OrderValidator(const std::map<int, std::vector<int>>& dependencies) : m_dependencies(dependencies) {}

bool OrderValidator::isCyclic(int v, std::vector<bool>& visited, std::vector<bool>& dfsVisited) {
    if (dfsVisited[v]) return true;

    if (!visited[v]) {
        visited[v] = true;
        dfsVisited[v] = true;

        for (int j = 0; j < m_dependencies[v].size(); ++j) {
            if (isCyclic(m_dependencies[v][j], visited, dfsVisited))
                return true;
        }
    }

    dfsVisited[v] = false;
    return false;
}

// Check if the order of filenames is correct
std::pair<bool, std::string> OrderValidator::isOrderCorrect(const std::vector<std::string>& filenames, FilenameMatcher& matcher)
{
    std::vector<int> res = { matcher.filenameToNumber(filenames[0]) };
    int temp = m_dependencies[matcher.filenameToNumber(filenames[0])].size();

    // Check for circular includes
    bool hasCycle = false;
    for (int i = 0; i < m_dependencies.size(); ++i) {
        std::vector<bool> visited(m_dependencies.size(), false);
        std::vector<bool> dfsVisited(m_dependencies.size(), false);
        if (!visited[i]) {
            if (isCyclic(i, visited, dfsVisited)) {
                hasCycle = true;
                break;
            }
        }
    }

    if (hasCycle) return std::make_pair(false, "Circular include detected; file processing is terminated.");

    // Iterate over filenames to check dependencies
    for (int i = 1; i < filenames.size(); ++i)
    {
        if (m_dependencies[matcher.filenameToNumber(filenames[i])].size() < temp)
        {
            return std::make_pair(false, "Incorrect file processing order.");
        } 
        else
        {
            temp = m_dependencies[matcher.filenameToNumber(filenames[i])].size();
        }

        // Check if dependencies are met
        for (const auto& value : m_dependencies[matcher.filenameToNumber(filenames[i])])
        {
            if (std::find(res.begin(), res.end(), value) == res.end())
            {
                return std::make_pair(false, "Incorrect file processing order.");
            }
        }
        res.push_back(matcher.filenameToNumber(filenames[i]));
    }

    return std::make_pair(true, "File processing order is correct.");
}
