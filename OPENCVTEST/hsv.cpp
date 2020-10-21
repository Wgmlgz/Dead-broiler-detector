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

Mat multipliy(Mat a, Mat b) {
    Mat ret = b.clone();

    for (int i = 0; i < ret.rows; i++) {
        for (int j = 0; j < ret.cols; j++) {
            uint8_t pix1 = a.at<Vec3b>(i, j)[0];
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
            int pix = (uint8_t)a.at<int8_t>(i, j) - ((uint8_t)b.at<int8_t>(i, j) + (uint8_t)c.at<int8_t>(i, j));
            if (pix < 0) pix = 0;
            if (pix > 255) pix = 255;
            ret.at<int8_t>(i, j) = pix;
        }
    }

    return ret;
}

int main2() {
    cout << "_________________________________" << endl;
    cout << "|                               |" << endl;
    cout << "|Dead broiler detector by Wgmlgz|" << endl;
    cout << "|_______________________________|" << endl;
    cout << endl;

    /* load stuff */
    // video load
    VideoCapture capture(samples::findFile("data to analyze\\vid.avi"));
    if (!capture.isOpened()) {
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
    int step = 30;

    // input values (uncomment if need)
    //cin >> start_frame;
    //cin >> end_frame;
    //cin >> step;

    cout << endl << "input complite or default values are used" << endl;

    /* main cycle */
    // skip frames
    Mat frame;
    for (int i = 0; i < start_frame; i += step) {
        Mat old_frame = frame;
        for (int j = 0; j < step; ++j) capture >> frame;
        if (frame.empty()) {
            frame = old_frame;
            break;
        }
    }

    // init summ buffer
    capture >> frame;
    vector<vector<uint64_t>> sum(frame.rows, vector<uint64_t>(frame.cols));

    // main loop
    for (int i = start_frame; i < end_frame; i += step) {
        // load frame
        cout << "analyzing frame " << i << endl;
        Mat old_frame = frame;
        for(int j = 0; j < step; ++j) capture >> frame;
        if (frame.empty()) {
            frame = old_frame;
            break;
        }

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
        string savingName = "tmp result\\" + to_string(i) + ".jpg";
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
    imwrite("result\\rough result.png", result);
    Mat mask = imread("data to analyze\\mask.png", IMREAD_GRAYSCALE);
    result = multipliy(result, mask);
    imwrite("result\\with mask.png", result);
    result = isolateChannel(result, 100, 255);
    imwrite("result\\with mask, isolate.png", result);
    blur(result, result, Size(30, 30));
    imwrite("result\\with mask, isolate, blur.png", result);
    result = isolateChannel(result, 90, 255);
    imwrite("result\\with mask, isolate, blur, isolate.png", result);
    cout << "end filter" << endl;


    // display result video
    destroyWindow("h");
    destroyWindow("s");
    destroyWindow("v");

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(result, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect objectBoundingRectangle = Rect(0, 0, 0, 0);

    VideoCapture capture1(samples::findFile("data to analyze\\vid.avi"));
    if (!capture1.isOpened()) {
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    for (int i = 0; true; i++) {
        cout << "displaying frame " << i << endl;
        capture1 >> frame;
        if (frame.empty()) {
            break;
        }

        putText(frame, "Total dead chicken: " + to_string(contours.size()), Point(50, 150), 1, 3, Scalar(255, 255, 255), 4);
        for (int i = 0; i < contours.size(); ++i) {
            objectBoundingRectangle = boundingRect(contours.at(i));

            int x = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
            int y = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;

            rectangle(frame,
                Rect(objectBoundingRectangle.x - 20,
                    objectBoundingRectangle.y - 20,
                    objectBoundingRectangle.width + 40,
                    objectBoundingRectangle.height + 40),
                Scalar(95, 105, 234), 4);
            line(frame, Point(x, y + 10), Point(x, y - 10), Scalar(125, 194, 119), 2);
            line(frame, Point(x + 10, y), Point(x - 10, y), Scalar(125, 194, 119), 2);
            putText(frame, "Chicken " + to_string(i), Point(objectBoundingRectangle.x - 30, objectBoundingRectangle.y - 30), 1, 3, Scalar(20, 20, 20), 4);
        }

        imshow("Main window (tmp frame vision / result)", frame);
        if (i == 0) imwrite("result\\result.png", frame);
        waitKey(30);
    }
    cout << "end output" << endl;

    // end
    cout << "end!" << endl;
    waitKey();
}
