#include <iostream>
#include "reflection.hpp"

#include "test_classes.hpp"

int main()
{
    std::cout <<  std::endl<<  std::endl << "REGISTERED TYPES ARE::" << std::endl;
    reflection::TypeTable::foreachType(reflection::printType);

    object* objects[] = {new shape(), new square(), new circle(), new animal(), new cat(), new dog()};

    std::cout << "REFLECTING INSTANCES::" << std::endl;
    for (unsigned int i = 0; i < sizeof(objects)/sizeof(objects[0]); i++)
    {
        reflection::TypeInfo *info =  reflection::dynamic_reflect(objects[i]);
        std::cout << "Type:" << info->name << std::endl;

        info->foreachField(reflection::printField);
    }

    return 0;
}
