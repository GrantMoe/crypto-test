#include <iostream>
#include <fstream>
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

    streampos size;
    char * memblock;

//    int keyByte = 0b11111111 & stoi("fb", nullptr, 16);
    auto keyByte = (uint8_t) stoi("fb", nullptr, 16);
    int plainByte = stoi("0x01", nullptr, 16);

    int cypherByte = keyByte ^ plainByte;

//    cout << keyByte << endl;
//    auto binTemp = static_cast<uint8_t >(keyByte);
//    auto binTemp = (uint8_t) keyByte;
    cout << sizeof(keyByte);
    cout << unsigned(keyByte) << endl;

//    cout << sizeof(keyByte);
//    char bintemp = 0xff & keyByte;
//    cout << sizeof(binTemp);
//    cout << binTemp << endl;
//    cout << keyByte << endl;
//    cout << plainByte << endl;
//    cout << cypherByte << endl;

//    ifstream file ("key.bin", ios::in|ios::binary|ios::ate);
//    if (file.is_open())
//    {
//        size = file.tellg();
//        memblock = new char [size];
//        file.seekg (0, ios::beg);
//        file.read (memblock, size);
//
//        file.close();
//
//        cout << "\n\nthe entire file content is in memory\n\n";
//
//        delete[] memblock;
//    }
//
//
//    else cout << "Unable to open file";
//

    return 0;
}