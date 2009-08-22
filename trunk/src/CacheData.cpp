#include "CacheData.h"
#include "MyAssert.h"

const char* CacheData::CACHE_DIR = "cache";

std::string CacheData::getThumbnailFile(){
  assert(hash.size() > 2);

  return std::string(CACHE_DIR) + "/"
         + hash.substr(0,2) + "/" + hash+".jpg";
}
