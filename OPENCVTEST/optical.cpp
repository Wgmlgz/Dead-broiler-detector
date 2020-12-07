#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
const char* params
= "{ help h         |           | Print usage }"
"{ input          | vtest.avi | Path to a video or a sequence of image }"
"{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";
int main33(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, params);
    parser.about("This program shows how to use background subtraction methods provided by "
        " OpenCV. You can process both videos and images.\n");
    if (parser.has("help"))
    {
        //print help information
        parser.printMessage();
    }
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    if (parser.get<String>("algo") == "MOG2")
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();
    VideoCapture capture("data to analyze\\vid.avi");
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open: " << parser.get<String>("input") << endl;
        return 0;
    }
    Mat frame, fgMask, old_frame;
    capture >> frame;
    namedWindow("Frame", WINDOW_FREERATIO);
    namedWindow("FG Mask", WINDOW_FREERATIO);
    namedWindow("Main window", WINDOW_FREERATIO);
    vector<vector<uint64_t>> sum(frame.rows, vector<uint64_t>(frame.cols));
    while (true) {
        old_frame = frame;
        capture >> frame;
        if (frame.empty()) {
            frame = old_frame;
            break;
        }
        //update the background model
        pBackSub->apply(frame, fgMask);
        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
            cv::Scalar(255, 255, 255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);

        for (int i = 0; i < fgMask.rows; i++)
            for (int j = 0; j < fgMask.cols; j++)
                sum[i][j] += (uint8_t)fgMask.at<int8_t>(i, j);
        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    double max = 0;
    for (int i = 0; i < sum.size(); i++)
        for (int j = 0; j < sum[0].size(); j++)
            if (sum[i][j] > max) max = sum[i][j];
    cout << "end max" << endl;

    // normalize
    Mat result = frame;
    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum[0].size(); j++) {
            int tmp = (((double)sum[i][j]) / max) * 255;
            result.at<Vec3b>(i, j)[0] = tmp;
            result.at<Vec3b>(i, j)[1] = tmp;
            result.at<Vec3b>(i, j)[2] = tmp;
        }
    }
    cout << "end normalize" << endl;
    imshow("Main window", result);

    waitKey();
    return 0;
}