#include "BitmapDistance.h"
#include "MyAssert.h"
#include "ColorDistance.h"


double BitmapDistance::distance(Bitmap* b1, Bitmap* b2, ColorAdjust*
        adjust) {
    Assert(b1->getWidth() == b2->getWidth(), "");
    Assert(b1->getHeight() == b2->getHeight(), "");

    ColorDistance cd;

    double sum = 0;
    int w = b1->getWidth();
    int h = b1->getHeight();

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++) {
            double cr = b2->getR(x,y);
            double cg = b2->getG(x,y);
            double cb = b2->getB(x,y);
            adjust->adjust(cr,cg,cb);

            sum += cd.distance(
                    b1->getR(x, y), b1->getG(x, y), b1->getB(x, y),
                    cr,cg,cb);
        }

    return 10000.0 * sum / w / h;
}
