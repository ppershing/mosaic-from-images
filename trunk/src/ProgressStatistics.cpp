// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "ProgressStatistics.h"
#include "MyStringUtils.h"

ProgressStatistics::ProgressStatistics(){
 total=0;
 clearIncremental();
}

void ProgressStatistics::clearIncremental(){
  imagesProcessed=0;
  imagesChanged=0;
  totalChange=0;
}

double ProgressStatistics::getAverageEnhancement(){
    return -totalChange/(imagesProcessed+1e-10);
}

double ProgressStatistics::getTotalDistance(){
    return total;
}

std::string ProgressStatistics::getStatistics(){
  std::string result;
  result = "Statistics: ";
  result += "changed: ";
  result += MyStringUtils::intToString(imagesChanged);
  result += "/" + MyStringUtils::intToString(imagesProcessed);
  result += " fit: ";
  result += MyStringUtils::doubleToString(totalChange,0);
  result += "/" + MyStringUtils::doubleToString(total,0);
  result += " avg: ";
  result +=
      MyStringUtils::doubleToString(totalChange/(imagesChanged+1e-10));
  
  return result;
}

void ProgressStatistics::nextImage(double delta){
 total+=delta;
 totalChange+=delta;
 if (delta!=0) imagesChanged++;
 imagesProcessed++;
}
