#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

    // process arguments
    std::string keyFile;
    for (int i = 1; i < argc; i++) {
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


    // open key file
    std::ifstream keyStream (keyFile, std::ios::in|std::ios::binary|std::ios::ate);

    if (keyStream.is_open()) {

        char * keyBytes;
        char * plainBytes;
        char * cypherBytes;

        // get key bytes and chunk size
        std::streampos chunkSize = keyStream.tellg();
        keyBytes = new char [chunkSize];
        plainBytes = new char [chunkSize];
        cypherBytes = new char [chunkSize];
        keyStream.seekg (0, std::ios::beg);
        keyStream.read (keyBytes, chunkSize);
        keyStream.close();

        int lastByte;  // count for times when plain data is not a multiple of chunk size

        // open stdin and stdout streams
        if (std::freopen("plain", "r", stdin)) { // CLion doesn't support command line redirects in debugging!
            std::freopen("cypher", "w", stdout);  // C'mon Jetbrains. Get it together.

            while (std::cin.good()) {

                // get chunk of plain data
                lastByte = 0;
                for (int i = 0; i < chunkSize; i++) {
                    if (std::cin.get(plainBytes[i])) {
                        lastByte++;
                    } else { // if there are fewer than chunkSize bytes in the last bit of plain data:
                        break;
                    }
                }

                // XOR-encrypt chunk with key
                if (!std::cin.eof()) {
                    for (int i = 0; i < lastByte; i++) {
                        cypherBytes[i] = plainBytes[i] ^ keyBytes[i];
                    }

//              write cyphered chunk to stdout
                    for (int i = 0; i < lastByte; i++) {
                        std::cout << cypherBytes[i];
                    }
                }
            }

            // close streams
            std::fclose(stdin);
            std::fclose(stdout);
        }

        delete[] keyBytes;
        delete[] plainBytes;
        delete[] cypherBytes;

    } else {
        std::cerr << "cannot get key";
        return 1;
    }

    return 0;
}