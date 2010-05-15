// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#ifndef H_CACHE_DATA
#define H_CACHE_DATA

#include <string>

class CacheData {
    public:
        std::string hash;
        std::string originalHash;
        std::string username;
        std::string imageFile;

        std::string getThumbnailFile();
    private:
        static const char* CACHE_DIR;
};

#endif
