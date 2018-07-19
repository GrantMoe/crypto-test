#include <iostream>
#include <fstream>
#include <bitset>
#include <climits>

int main(int argc, char *argv[]) {

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
    std::ifstream keyStream(keyFile, std::ios::in | std::ios::binary | std::ios::ate);

    if (keyStream.is_open()) {
        // get key chunk size
        auto chunkSize = (unsigned int) keyStream.tellg();
        // get key
        auto tempBytes = new char[chunkSize];
        keyStream.seekg(0, std::ios::beg);
        keyStream.read(tempBytes, chunkSize);

        // i'm not certain I needed to do this, but it feels better:
        unsigned char *keyBytes;
        keyBytes = new unsigned char[chunkSize];
        for (int i = 0; i < chunkSize; i++) {
            keyBytes[i] = (unsigned char) tempBytes[i];
        }
        delete[] tempBytes;
        keyStream.close();

        // open stdin and stdout streams
        if (std::freopen("plain", "r", stdin)) { // CLion doesn't support command line redirects for debugging!
            std::freopen("cypher", "w", stdout);  // C'mon Jetbrains. Get it together.

            char plainByte;
            auto plainBytes = new unsigned char[chunkSize];
            auto cypherBytes = new unsigned char[chunkSize];
            int lastByte;  // count for times when plain data is not a multiple of chunk size

            unsigned int rotate_bit;

            while (std::cin.good()) {
                // get chunk of plain data
                lastByte = 0;
                for (int i = 0; i < chunkSize; i++) {
                    if (std::cin.get(plainByte)) {
                        plainBytes[i] = (unsigned char) plainByte;
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

                // rotate key
                rotate_bit = ((unsigned) keyBytes[chunkSize - 1] >> (unsigned) (CHAR_BIT - 1)) & (unsigned) 0x01;
                for (int i = chunkSize - 1; i > 0; i--) {
                    // rotate each byte in the key:
                    keyBytes[i] = (unsigned char) ((unsigned) (keyBytes[i] << 1u) |
                                                   ((unsigned) (keyBytes[i - 1] >> (unsigned) (CHAR_BIT - 1)) &
                                                    (unsigned) 0x01));
                }
                keyBytes[0] = (unsigned char) (((unsigned) keyBytes[0] << 1u) | rotate_bit);
            }

            delete[] plainBytes;
            delete[] cypherBytes;
            // close streams
            std::fclose(stdin);
            std::fclose(stdout);
        }

        delete[] keyBytes;

    } else {
        std::cerr << "cannot get key";
        return 1;
    }
    return 0;
}