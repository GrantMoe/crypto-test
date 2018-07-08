#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    std::string keyFile;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-k") {
            if (i + 1 < argc) {
                keyFile = argv[++i];
                std::cout << keyFile;
            } else {
                std::cerr << "-k option requires argument";
                return 1;
            }
        }
        // thread count argument handling goes here
    }





    return 0;
}