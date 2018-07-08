#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    std::string keyFile;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-k") {
            if (i + 1 < argc) {
                keyFile = argv[++i];
            } else {
                std::cerr << "-k option requires argument";
                return 1;
            }
        }
        // thread count argument handling goes here
    }

    std::streampos size;
    char * memblock;
    std::ifstream file (keyFile, std::ios::in|std::ios::binary|std::ios::ate);

    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);

        file.close();

        std::cout << memblock;

        std::cout << "\n\nthe entire file content is in memory\n\n";

        delete[] memblock;
    }

    else std::cout << "Unable to open file";

    return 0;
}