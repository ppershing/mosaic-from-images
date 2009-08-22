// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk

#include "Version.h"
#include "MyStringUtils.h"
#include "Mosaic.ver"

// {{{ getMajorNumber
int Version::getMajorNumber(){
    return MAJOR;
}
// }}}

// {{{ getMinorNumber
int Version::getMinorNumber(){
    return MINOR;
}
// }}}

// {{{ getBuildNumber
int Version::getBuildNumber(){
    return BUILD;
}
// }}}

// {{ getBuildName
std::string Version::getBuildName(){
    return std::string(BUILD_NAME);
}
// }}}

// {{ getBuildName
std::string Version::getBuildDate(){
    return std::string(__DATE__)+" "+std::string(__TIME__);
}
// }}}

// {{{ getBuildState
std::string Version::getBuildState(){
    return std::string(BUILD_STATE);
}
// }}}

// {{{ getVersionString
std::string Version::getVersionString(){
    return  MyStringUtils::intToString(getMajorNumber())+
        "."+MyStringUtils::intToString(getMinorNumber())+
        "."+MyStringUtils::intToString(getBuildNumber());
}
// }}}


// {{{ getAuthors
std::string Version::getAuthors(){
    return std::string("Peter Peresini");
}
// }}}

// {{{ getProductName
std::string Version::getProductName(){
    return "Mosaic of images";
}
// }}}

// {{{ getFullProductName
std::string Version::getFullProductName(){
    return getProductName()+" version "+
           getVersionString()+" ("+
           getBuildName()+" "+
           getBuildState()+" build at "+
           getBuildDate()+"), Authors: "+
           getAuthors();
}
//}}}
