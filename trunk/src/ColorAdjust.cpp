// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
#include "ColorAdjust.h"
#include "NoAssert.h"
#include "MyStringUtils.h"
#include <vector>

const double ColorAdjust::min[ColorAdjust::CNT] = {-0.1,0.9,0.95,0.95,-0.1};
const double ColorAdjust::max[ColorAdjust::CNT] = {0.1,1.1,1.05,1.05,0.1};
const double ColorAdjust::start[ColorAdjust::CNT] = {0,1,1,1,0};
const std::string ColorAdjust::name[ColorAdjust::CNT] =
 {"black point", "white point", "red adjust", "blue adjust", "saturation"};


std::string ColorAdjust::toString(){
  std::string result;
  for (int x=0; x<CNT; x++) {
      if (x) result+=",";
    result+=MyStringUtils::doubleToString(variables[x],4);
  }
  return result;
}

void ColorAdjust::fromString(const std::string& str){
    if (str=="") {
        for (int x=0; x<CNT; x++) variables[x]=start[x];
        return;
    }

    std::vector<std::string> tokens = MyStringUtils::splitString(str, ",");
  Assert(tokens.size() == CNT, "");

  for (int x=0; x<CNT; x++)
      variables[x]=MyStringUtils::stringToDouble(tokens[x]);

  check();
}

double ColorAdjust::constrain(double x){
    if (x<0) x=0;
    if (x>1) x=1;
    return x;
}

ColorAdjust::ColorAdjust(){
    variables.resize(CNT);
 for (int q=0; q<CNT; q++){
    variables[q]=start[q];
 }
}

inline void ColorAdjust::saturation(double &r, double &g, double &b){
 double c[3]={r,g,b};
 int mmin=0;
 int mmax=0;
 if (g<c[mmin]) mmin=1;
 if (b<c[mmin]) mmin=2;
 if (g>c[mmax]) mmax=1;
 if (b>c[mmax]) mmax=2;

 double s=c[mmax]-c[mmin];
 double dx = s*variables[4];
 if (c[mmax]+dx>1) dx=1-c[mmax];
 if (c[mmin]-dx<0) dx=c[mmin];

 c[mmax]+=dx;
 c[mmin]-=dx;

 r=c[0];
 g=c[1];
 b=c[2];
}

inline void ColorAdjust::check(){
  for (int q=0; q<CNT; q++) {
    Assert(variables[q]>=min[q], "");
    Assert(variables[q]<=max[q], "");
  }
}

inline double ColorAdjust::brightnessContrast(double x){
    return (variables[1]-variables[0])*x+variables[0];
}

void ColorAdjust::adjust(double &r, double &g, double &b){
    check();
    saturation(r,g,b);
    r = constrain(brightnessContrast(r)*variables[2]);
    g = constrain(brightnessContrast(g));
    b = constrain(brightnessContrast(b)*variables[3]);
}

