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
