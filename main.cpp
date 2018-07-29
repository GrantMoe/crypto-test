#include <iostream>
#include <fstream>
#include <climits>
//#include <boost/dynamic_bitset.hpp>

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
        // thread count argument handling goes here probably
    }

    // open input binary stream with place indicator at the end of the stream
    std::ifstream keyStream(keyFile, std::ios::in | std::ios::binary | std::ios::ate);

    // load up key byte array
    if (keyStream.is_open()) {
        // use the stream position indicator (at end of stream) to determine key size
        auto chunkBytes = (unsigned int) keyStream.tellg();
        auto tempBytes = new char[chunkBytes];
        // grab key-size bytes from the key file
        keyStream.seekg(0, std::ios::beg);
        keyStream.read(tempBytes, chunkBytes);

        // debug
//        boost::dynamic_bitset<unsigned char> firstKey;
//        boost::dynamic_bitset<unsigned char> currentKey;
//        boost::dynamic_bitset<unsigned char> tempBits;

        // i'm not certain I needed to do this, but it feels better:
        unsigned char *keyBytes;
        keyBytes = new unsigned char[chunkBytes];
        for (int i = 0; i < chunkBytes; i++) {
            keyBytes[i] = (unsigned char) tempBytes[i];
            // debug
//            firstKey.append(keyBytes[i]);
        }
        delete[] tempBytes;
        keyStream.close();

//        std::clog << "first key:   " << firstKey << std::endl;

        // open stdin and stdout streams
        if (std::freopen("plain", "r", stdin)) { // CLion doesn't support command line redirects for debugging!
            std::freopen("cypher", "w", stdout);  // C'mon Jetbrains. Get it together.

            char plainByte;
            auto plainBytes = new unsigned char[chunkBytes];
            auto cypherBytes = new unsigned char[chunkBytes];
            int lastByte;  // count for times when plain data is not a multiple of chunk size

            unsigned char rotate_mask;  // used to store least-significant bit of least-significant key byte

            while (std::cin.good()) {
                // get chunk of plain data
                lastByte = 0;
                for (int i = 0; i < chunkBytes; i++) {
                    if (std::cin.get(plainByte)) {
                        plainBytes[i] = (unsigned char) plainByte;
                        lastByte++;
                    } else { // if there are fewer than chunkBytes bytes in the last bit of plain data:
                        break;
                    }
                }
                // XOR-encrypt chunk of plain data bytes with key bytes
                if (!std::cin.eof()) {
                    for (int i = 0; i < lastByte; i++) {
                        cypherBytes[i] = plainBytes[i] ^ keyBytes[i];
                    }
                // write cyphered chunk of bytes to cypher file
                    for (int i = 0; i < lastByte; i++) {
                        std::cout << cypherBytes[i];
                    }
                }

                // rotate key
                // get LSBit of LSByte
                rotate_mask = keyBytes[chunkBytes - 1] >> (CHAR_BIT - 1);
                // shift each key byte left. grab each next-less-significant byte's MSB to use as current byte's LSB
                for (int i = (chunkBytes - 1); i >= 0; i--) {
                    keyBytes[i] = (keyBytes[i] << 1u) | (i >= 1 ? (keyBytes[i - 1] >> (CHAR_BIT - 1)) : rotate_mask);
                }

                // debug
//                currentKey.clear();
//                for (int i = 0; i < chunkBytes; i++) {
//                    currentKey.append(keyBytes[i]);
//                }
//                std::clog << "current key: " << currentKey << std::endl;
            }

            // delete byte arrays
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