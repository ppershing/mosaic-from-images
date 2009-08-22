#include "ThumbnailFitData.h"
#include "ThumbnailStack.h"
#include "MyStringUtils.h"

ThumbnailFitData::ThumbnailFitData(){
  diffStack.resize(ThumbnailStack::SIZE);
  diffStackOriginal.resize(ThumbnailStack::SIZE);
  diff=0;
  for (int q=0; q<ThumbnailStack::SIZE; q++)
      diffStack[q]=diffStackOriginal[q]=0;
}

std::string ThumbnailFitData::getFitStatistics(){
  std::string result;
  for (int q=0; q<ThumbnailStack::SIZE; q++) {
      if (q) result+=",";
      result+=MyStringUtils::doubleToString(diffStack[q],2);
  }

  result+=",original";
  for (int q=0; q<ThumbnailStack::SIZE; q++) {
      if (q) result+=",";
      result+=MyStringUtils::doubleToString(diffStackOriginal[q],2);
  }

  return result;
}
