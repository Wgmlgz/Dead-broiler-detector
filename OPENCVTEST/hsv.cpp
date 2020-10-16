#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

Mat _hsv[3], frame_hsv, _cum[3];
vector<Mat> buff;

const int slider_max = 255;

// 0 52
int sliderHmin = 0;
// 0 119
int sliderSmin = 0;
// 170 211
int sliderVmin = 0;

int sliderHmax = 255;
int sliderSmax = 255;
int sliderVmax = 255;


int sliderHmin_ = 0;
int sliderSmin_ = 0;
int sliderVmin_ = 0;

int sliderHmax_ = 255;
int sliderSmax_ = 255;
int sliderVmax_ = 255;

Mat isolateChannel(Mat input, int min, int max) {
    Mat ret = input.clone();

    for (int i = 0; i < ret.rows; i++) {
        for (int j = 0; j < ret.cols; j++) {
            uint8_t pix = ret.at<int8_t>(i, j);
            //pix += 128;
            int res = 255;
            if (pix < min) {
                res = 0;
            }
            else if (pix > max) {
                res = 0;
            }
            ret.at<int8_t>(i, j) = res;
        }
    }

    return ret;
}

Mat substractMat(Mat a, Mat b, Mat c) {
    Mat ret = a.clone();

    for (int i = 0; i < ret.rows; i++) {
        for (int j = 0; j < ret.cols; j++) {
            uint8_t pix = a.at<int8_t>(i, j) - (b.at<int8_t>(i, j) + c.at<int8_t>(i, j));

            ret.at<int8_t>(i, j) = pix;
        }
    }

    return ret;
}
static void trackbarHmin(int, void*){
    if (sliderHmin_ == sliderHmin) return;
    imshow("h", isolateChannel(_hsv[0], sliderHmin, sliderHmax));
    sliderHmin_ = sliderHmin;
}
static void trackbarSmin(int, void*) {
    if (sliderSmin_ == sliderSmin) return;
    imshow("s", isolateChannel(_hsv[1], sliderSmin, sliderSmax));
    sliderSmin_ = sliderSmin;
}
static void trackbarVmin(int, void*) {
    if (sliderVmin_ == sliderVmin) return;
    imshow("v", isolateChannel(_hsv[2], sliderVmin, sliderVmax));
    sliderVmin_ = sliderVmin;
}

static void trackbarHmax(int, void*) {
    if (sliderHmax_ == sliderHmax) return;
    imshow("h", isolateChannel(_hsv[0], sliderHmin, sliderHmax));
    sliderHmax_ = sliderHmax;
}
static void trackbarSmax(int, void*) {
    if (sliderSmax_ == sliderSmax) return;
    imshow("s", isolateChannel(_hsv[1], sliderSmin, sliderSmax));
    sliderSmax_ = sliderSmax;
}
static void trackbarVmax(int, void*) {
    if (sliderVmax_ == sliderVmax) return;
    imshow("v", isolateChannel(_hsv[2], sliderVmin, sliderVmax));
    sliderVmax_ = sliderVmax;
}

int main() {
    /* load stuff */

    VideoCapture capture(samples::findFile("vid.avi"));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    namedWindow("Cum", WINDOW_FREERATIO);
    namedWindow("h", WINDOW_FREERATIO);
    namedWindow("s", WINDOW_FREERATIO);
    namedWindow("v", WINDOW_FREERATIO);


    /* main cycle */
    cout << "enter frames numbers: ";
    int start_frame = 200;
    int end_frame = 202;
    //cin >> do_frames;

    for (int i = 0; i < start_frame; ++i) {
        Mat frame;
        capture >> frame;
        if (frame.empty())
            break;
    }
    for (int i = start_frame; i < end_frame; ++i) {
        // load frame
        cout << "frame " << i << endl;
        Mat frame;
        capture >> frame;
        if (frame.empty())
            break;

        // to hsv
        cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
        split(frame_hsv, _hsv);

        _hsv[0] = isolateChannel(_hsv[0], 162, 194);
        _hsv[1] = isolateChannel(_hsv[1], 136, 255);
        _hsv[2] = isolateChannel(_hsv[2], 44, 85);

        // show
        imshow("h", _hsv[0]);
        imshow("s", _hsv[1]);
        imshow("v", _hsv[2]);

        Mat cum = substractMat(_hsv[0], _hsv[1], _hsv[2]);
        imshow("Cum", cum);
        buff.push_back(cum);
        //createTrackbar("H min", "h", &sliderHmin, slider_max, trackbarHmin);
        //createTrackbar("S min", "s", &sliderSmin, slider_max, trackbarSmin);
        //createTrackbar("V min", "v", &sliderVmin, slider_max, trackbarVmin);
        //
        //createTrackbar("H max", "h", &sliderHmax, slider_max, trackbarHmax);
        //createTrackbar("S max", "s", &sliderSmax, slider_max, trackbarSmax);
        //createTrackbar("V max", "v", &sliderVmax, slider_max, trackbarVmax);
        //
        //trackbarHmin(sliderHmin, 0);
        //trackbarSmin(sliderSmin, 0);
        //trackbarVmin(sliderVmin, 0);
        //
        //trackbarHmin(sliderHmax, 0);
        //trackbarSmin(sliderSmax, 0);
        //trackbarVmin(sliderVmax, 0);
        //
        //waitKey(100000);
    }

    cout << "end scan" << endl;
    vector<vector<uint8_t>> sum(buff[0].rows, vector<uint8_t>(buff[0].cols));
    cout << "vector complite" << endl;
    for (int k = 0; k < buff.size(); ++k) {
        cout << "sum frame " << to_string(k) << endl;
        for (int i = 0; i < buff[k].rows; i++) {
            for (int j = 0; j < buff[k].cols; j++) {
                sum[i][j] += buff[k].at<int8_t>(i, j);
            }
        }
    }
    cout << "end sum" << endl;
    double max = 0;
    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum[0].size(); j++) {
            if (sum[i][j] > max) {
                max = sum[i][j];
            }
        }
    }
    cout << "end max" << endl;

    Mat result = buff[0];
    for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < sum[0].size(); j++) {
            int tmp = (((double)sum[i][j]) / max) * 255;
            result.at<int8_t>(i, j) = tmp;
            result.at<int8_t>(i, j) = tmp;
            result.at<int8_t>(i, j) = tmp;
        }
    }
    cout << "end res" << endl;
    imshow("Cum", result);
    imwrite("shvCUM!!!!!!!.png", result);
    waitKey();
}
