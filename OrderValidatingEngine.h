#pragma once

#ifndef ORDERVALIDATINGENGINE_H
#define ORDERVALIDATINGENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "FilenameMatcher.h"
#include "DependencyProvider.h"
#include "OrderValidator.h"

class OrderValidatingEngine
{
public:
    // Execution of the methods above
    void execute(std::istream& in, std::ostream& out);
};

#endif // ORDERVALIDATINGENGINE_H