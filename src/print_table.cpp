#include <iostream>

void print_T(int values) {
    for (int i = 0; i != values; ++i) {
        for (int j = 0; j != i; ++j) {
            std::cout << "0, ";
        }
        for (int j = i; j != values; ++j) {
            std::cout << "+1, ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i != values; ++i) {
        for (int j = 0; j != i; ++j) {
            std::cout << "0, ";
        }
        for (int j = i; j != values; ++j) {
            std::cout << "-1, ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2)
        return 1;

    int values = std::atoi(argv[1]);

    print_T(values);
    return 0;
}