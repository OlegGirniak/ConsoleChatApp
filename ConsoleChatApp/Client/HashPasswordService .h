#include <openssl/sha.h>
#include <cstring>
#include <sstream>

using std::string;

class HashPasswordService
{
public:
    static string Hash(const std::string& password) 
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, password.c_str(), password.length());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
        {
            ss << std::hex << (int)hash[i];
        }

        return ss.str();
    }
};