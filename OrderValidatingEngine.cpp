#include "OrderValidatingEngine.h"

void OrderValidatingEngine::execute(std::istream& in, std::ostream& out)
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
    } else
    {
        out << result.second << std::endl;
    }
}