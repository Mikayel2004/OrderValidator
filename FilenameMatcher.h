#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

class FilenameMatcher
{
private:
    std::unordered_map<std::string, int> filenameToNumberMap;
    std::vector<std::string> numberToFilenameMap;

public:
    // Constructor to initialize FilenameMatcher
    FilenameMatcher(const std::string& directoryPath);

    // Convert filename to its corresponding number
    int filenameToNumber(const std::string& filename);

    // Convert number to its corresponding filename
    std::string numberToFilename(int number);
};
