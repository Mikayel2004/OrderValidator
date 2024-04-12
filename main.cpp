#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "FilenameMatcher.h"
#include "DependencyProvider.h"
#include "OrderValidator.h"
#include "OrderValidatingEngine.h"

int main()
{
    OrderValidatingEngine orderValidatingEngine;
    orderValidatingEngine.execute(std::cin, std::cout);
    return 0;
}