#ifndef H_CACHE
#define H_CACHE

#include <vector>
#include <string>
#include <map>
#include "CacheData.h"

class Cache{
  public:
    static int getTileWidth(int division);
    static int getTileHeight(int division);

    void loadFromFile(const std::string& filename);
    void clear();
    CacheData& getCacheData(const std::string& filehash);
    std::vector<std::string> enumerate();
    int getSize();
  private:
    void addFromLine(const std::string& line);
    std::map<std::string, CacheData> data;
    static const int TH_WIDTH = 200;
    static const int TH_HEIGHT = 150;
};
#endif
