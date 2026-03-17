#include <iostream>
#include "cube.h"
#include "boiling_soup.h"

int main(int argc, char** argv)
{
    while (true)
    {
        int choice;

        std::cout << "Vyberi programmu:\n";
        std::cout << "1 - Cube\n";
        std::cout << "2 - Boiling soup\n";
        std::cout << "0 - Exit\n";
        std::cout << "> ";
        std::cin >> choice;

        if (choice == 1)
        {
            runCube();
        }
        else if (choice == 2)
        {
            runBoilingSoup(argc, argv);
        }
        else if (choice == 0)
        {
            break;
        }
        else
        {
            std::cout << "Nevernyy vybor\n";
        }
    }

    return 0;
}