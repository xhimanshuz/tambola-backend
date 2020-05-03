#include "BrainEngine.h"

BrainEngine::BrainEngine()
{
    pickedNumber = std::make_shared<std::vector<int>>();

    cueNumber = std::make_shared<std::vector<int>>();
    for(int i=0;i<90; i++)
        cueNumber->push_back(i+1);
//    std::cout<< leftedNumber()<<std::endl;

}

int BrainEngine::pickNumber()
{
    srand(time(0));

    if(cueNumber->size() == 0)
    {
        for(int i=0;i<90; i++)
            cueNumber->push_back(i+1);
        pickedNumber->clear();
        return 0;
    }
    int number = cueNumber->at(rand() % cueNumber->size());
    cueNumber->erase(std::find(cueNumber->begin(), cueNumber->end(), number));
    pickedNumber->push_back(number);

    if(pickedNumber)
    return number;
}

std::string BrainEngine::leftedNumber()
{
    std::string str = "[";
    for(char i: *cueNumber)
        str+=std::to_string(i)+',';
    str[str.size()-1]=']';
    return str;
}
