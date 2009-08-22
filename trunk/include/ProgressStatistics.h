#ifndef H_PROGRESS_STATISTICS
#define H_PROGRESS_STATISTICS

#include <string> 

class ProgressStatistics {
    public:
        ProgressStatistics();
        void clearIncremental();
        std::string getStatistics();
        void nextImage(double delta);
        double getAverageEnhancement();
        double estimateFinalDiff(int images_remaining);
    private:
        int imagesProcessed;
        int imagesChanged;
        double totalChange;
        double total;

};

#endif
