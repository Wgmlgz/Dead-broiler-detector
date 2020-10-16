#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

Mat _hsv[3], frame_hsv, result;

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

Mat multipliMat(Mat a, Mat b) {
    Mat ret = a.clone();

    for (int i = 0; i < ret.rows; i++) {
        for (int j = 0; j < ret.cols; j++) {
            uint8_t pix1 = a.at<int8_t>(i, j);
            uint8_t pix2 = b.at<int8_t>(i, j);

            double pd1 = pix1;
            double pd2 = pix2;

            pd1 /= 255;
            pd2 /= 255;

            pd1 *= pd2;
            pd1 *= 255;

            ret.at<int8_t>(i, j) = pd1;

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

int main() {
    cout << "_________________________________" << endl;
    cout << "|                               |" << endl;
    cout << "|Dead broiler detector by Wgmlgz|" << endl;
    cout << "|_______________________________|" << endl;
    cout << endl;

    /* load stuff */
    // video load
    VideoCapture capture(samples::findFile("vid.avi"));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }

    // windows initialization
    namedWindow("Main window (tmp frame vision / result)", WINDOW_FREERATIO);
    namedWindow("h", WINDOW_FREERATIO);
    namedWindow("s", WINDOW_FREERATIO);
    namedWindow("v", WINDOW_FREERATIO);


    // setup values
    cout << endl << "enter start_frame, end_frame, step: ";
    int start_frame = 0;
    int end_frame = 700;
    int step = 1;
    
    // input values (uncomment if need)
    //cin >> start_frame;
    //cin >> end_frame;
    //cin >> step;

    cout << endl << "input complite or default values are used" << endl;

    /* main cycle */
    // skip frames
    Mat frame;
    for (int i = 0; i < start_frame; i += step) {
        capture >> frame;
        if (frame.empty())
            break;
    }

    // init summ buffer
    capture >> frame;
    vector<vector<uint64_t>> sum(frame.rows, vector<uint64_t>(frame.cols));

    // main loop
    for (int i = start_frame; i < end_frame; i += step) {
        // load frame
        cout << "analizing frame " << i << endl;
        capture >> frame;
        if (frame.empty()) break;

        // to hsv
        cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
        split(frame_hsv, _hsv);
        _hsv[0] = isolateChannel(_hsv[0], 162, 194);
        _hsv[1] = isolateChannel(_hsv[1], 136, 255);
        _hsv[2] = isolateChannel(_hsv[2], 44, 85);

        // show hsv
        imshow("h", _hsv[0]);
        imshow("s", _hsv[1]);
        imshow("v", _hsv[2]);

        // show and save tmp result
        Mat tmp_res = substractMat(_hsv[0], _hsv[1], _hsv[2]);
        imshow("Main window (tmp frame vision / result)", tmp_res);
        string savingName = to_string(i) + ".jpg";
        imwrite(savingName, tmp_res);
        
        // add value to summ buffer
        for (int i = 0; i < tmp_res.rows; i++)
            for (int j = 0; j < tmp_res.cols; j++)
                sum[i][j] += (uint8_t)tmp_res.at<int8_t>(i, j);

        waitKey(1);
    }
    cout << "end scan" << endl;


    /* result */
    // find max value
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

    // filter
    Mat mask = imread("mask.png", IMREAD_GRAYSCALE);
    result = multipliMat(mask, result);
    result = isolateChannel(result, 100, 255);
    blur(result, result, Size(30, 30));
    result = isolateChannel(result, 90, 255);
    cout << "end filter" << endl;

    // output
    imshow("Main window (tmp frame vision / result)", result);
    imwrite("result.png", result);
    cout << "end output" << endl;

    // end
    cout << "end!" << endl;
    waitKey();
}
