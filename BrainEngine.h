#ifndef BRAINENGINE_H
#define BRAINENGINE_H

#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <iostream>

class BrainEngine
{
    std::shared_ptr<std::vector<int>> pickedNumber;
    std::shared_ptr<std::vector<int>> cueNumber;
public:
    BrainEngine();

    int pickNumber();
    std::string leftedNumber();
};

#endif // BRAINENGINE_H
