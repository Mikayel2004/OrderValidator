#include "FilenameMatcher.h"

FilenameMatcher::FilenameMatcher(const std::string& directoryPath)
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

int FilenameMatcher::filenameToNumber(const std::string& filename)
{
    return filenameToNumberMap[filename];
}

std::string FilenameMatcher::numberToFilename(int number)
{
    return numberToFilenameMap[number];
}