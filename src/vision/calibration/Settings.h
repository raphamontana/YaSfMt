#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Settings
{
public:
    Settings();

    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

    enum InputType { INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST };

    /**
     * Write serialization for this class
     */
    void write( FileStorage& fs ) const;

    /**
     * Read serialization for this class
     */
    void read( const FileNode& node );

    void interprate();

    Mat nextImage();

    bool readStringList( const string& filename, vector<string>& l );

public:
    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->

    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;

private:
    string patternToUse;

};

#endif // SETTINGS_H
