#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int main1()
{
    VideoCapture capture(samples::findFile("vid.avi"));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    capture.set(CAP_PROP_FRAME_WIDTH, 50);
    capture.set(CAP_PROP_FRAME_HEIGHT, 50);
    Mat frame1, prvs;
    capture >> frame1;
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    vector<Mat> buff;

    //namedWindow("magnitude", WINDOW_FREERATIO);
    //namedWindow("angle", WINDOW_FREERATIO);
    namedWindow("Cum", WINDOW_FREERATIO);
    Mat white;
    white.setTo(cv::Scalar(0, 0, 0));

    cout << "enter frames numbers: ";
    int do_frames;
    cin >> do_frames;
    for (int i = 0; i < do_frames; ++i) {
        cout << "frame " << i << endl;
        Mat frame2, next;
        capture >> frame2;
        if (frame2.empty())
            break;
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));

        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = magn_norm;
        _hsv[1] = magn_norm;
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        //COLOR_HSV2BGR
        cvtColor(hsv8, bgr, COLOR_RGB2BGR);
        imshow("Cum", bgr);
        string savingName = to_string(i) + ".jpg";
        buff.push_back(bgr);
        imwrite(savingName, bgr);
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
        prvs = next;
    }
    cout << "end scan" << endl;
    vector<vector<int64_t>> sum(buff[0].rows, vector<int64_t>(buff[0].cols));
    cout << "vector complete" << endl;
    for (int k = 0; k < buff.size(); ++k) {
        cout << "sum frame " << to_string(k) << endl;
        for (int i = 0; i < buff[k].rows; i++) {
            for (int j = 0; j < buff[k].cols; j++) {
                sum[i][j] += buff[k].at<Vec3b>(i, j)[0];
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
            result.at<Vec3b>(i, j)[0] = tmp;
            result.at<Vec3b>(i, j)[1] = tmp;
            result.at<Vec3b>(i, j)[2] = tmp;
        }
    }
    cout << "end res" << endl;
    imshow("Cum", result);
    imwrite("CUM!!!!!!!.png", result);
    waitKey();
}