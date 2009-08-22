#ifndef H_COLOR_ADJUST
#define H_COLOR_ADJUST

#include <string>
#include <vector>

class ColorAdjust {
    public:
        static const int CNT=5;
        static const double min[CNT];
        static const double max[CNT];
        static const double start[CNT];
        static const std::string name[CNT];

        std::vector<double> variables;
        ColorAdjust();

        void adjust(double& r, double& g, double& b);

        std::string toString();
        void fromString(const std::string& str);

    private:
        void saturation(double &r, double &g, double &b);
        double brightnessContrast(double x);
        double constrain(double x);
        void check();
};

#endif
