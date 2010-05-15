// created and maintained by ppershing
// please report any bug or suggestion to ppershing<>fks<>sk
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
        double getTotalDistance();
    private:
        int imagesProcessed;
        int imagesChanged;
        double totalChange;
        double total;

};

#endif
