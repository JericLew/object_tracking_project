#include "MOTCorrelationTracker.h"

int main(int argc, char *argv[])
{
    int64 start = cv::getTickCount();

    // program input parameters
    string directory_name;
    string source_path;
    string tracker_name;

    // Check if all three arguments are provided
    if (argc < 4)
    {
        cout << "Please provide /path/to/tracking_ws/, /path/to/source and tracker name." << endl;
        return 1;
    }

    // Get the directory name, source path, and tracker name from the arguments
    directory_name = argv[1];
    source_path = argv[2];
    tracker_name = argv[3];

    MOTCorrelationTracker tracker;
    tracker.inputPaths(directory_name, source_path, tracker_name);
    tracker.initTracker();
    tracker.runObjectTracking();

    int64 end = cv::getTickCount();
    double elapsedTime = (end - start) / cv::getTickFrequency();
    cout << "Total Elapsed Time: " << elapsedTime << " s" << std::endl;
    cout << endl;

    return 0;
}
