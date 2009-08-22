#include "Cache.h"
#include <fstream>
#include "MyStringUtils.h"
#include "Exceptions.h"
#include "MyAssert.h"
#include "Debug.h"
#include <algorithm>

int Cache::getTileWidth(int division){
  Assert(division < 8, "");
  return TH_WIDTH >> division;
}

int Cache::getTileHeight(int division){
  Assert(division < 8, "");
  return TH_HEIGHT >> division;
}

void Cache::loadFromFile(const std::string& filename){
    DEBUG("loading cache from file");
    std::ifstream f;
    f.open(filename.c_str());
    if (f.fail()) throw ECantOpenFile("Cache::LoadFromFile");


    while(!f.eof()) {
        char str[1000];
        str[0]=0;
        f.getline(str,1000);

        std::string s(str);
        addFromLine(s);
    }
    DEBUG("loaded " + MyStringUtils::intToString(getSize()) +
            " thumbnails");
}

void Cache::addFromLine(const std::string& line){
    if (line == "") return;
//    DEBUG("loading line '"+line+"'");
    CacheData cdata;
    std::vector<std::string> tokens;
    tokens = MyStringUtils::splitString(line,"|");

    Assert(tokens.size()==4, "problem with line '"+line+"'");
    
    cdata.hash = tokens[0];
    cdata.originalHash = tokens[1];
    cdata.username = tokens[2];
    cdata.imageFile = tokens[3];

    // insert into map
    data[cdata.hash] = cdata;
}

int Cache::getSize(){
    return data.size();
}

std::vector<std::string> Cache::enumerate(){
  std::vector<std::string> result;
  std::map<std::string, CacheData>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
      result.push_back(it->first);
  }
  return result;
}

CacheData& Cache::getCacheData(const std::string& hash) {
    Assert(data.find(hash)!=data.end(), "");
    return data[hash];
}
