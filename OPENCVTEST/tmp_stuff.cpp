/*

const int slider_max = 255;

// 162 194
int sliderHmin = 0;
// 136 255
int sliderSmin = 0;
// 44 85
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

int slider_min_ = 0;
int slider_max_ = 255;

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

static void trackbar_min(int, void*) {
    //if (sliderVmax_ == slider_max) return;
    imshow("Window", isolateChannel(result, slider_min_, slider_max_));
    //sliderSmax_ = slider_max;
}
static void trackbar_max(int, void*) {
    //if (sliderVmax_ == slider_max) return;
    imshow("Window", isolateChannel(result, slider_min_, slider_max_));
    //slider_max_ = slider_max;
}

createTrackbar("H min", "h", &sliderHmin, slider_max, trackbarHmin);
createTrackbar("S min", "s", &sliderSmin, slider_max, trackbarSmin);
createTrackbar("V min", "v", &sliderVmin, slider_max, trackbarVmin);

createTrackbar("H max", "h", &sliderHmax, slider_max, trackbarHmax);
createTrackbar("S max", "s", &sliderSmax, slider_max, trackbarSmax);
createTrackbar("V max", "v", &sliderVmax, slider_max, trackbarVmax);

trackbarHmin(sliderHmin, 0);
trackbarSmin(sliderSmin, 0);
trackbarVmin(sliderVmin, 0);

trackbarHmin(sliderHmax, 0);
trackbarSmin(sliderSmax, 0);
trackbarVmin(sliderVmax, 0);

createTrackbar("min", "Window", &slider_min_, slider_max, trackbar_min);
createTrackbar("max", "Window", &slider_max_, slider_max, trackbar_max);
trackbar_min(slider_min_, 0);
trackbar_min(slider_max_, 0);

result = imread("shvCUM!!!!!!!.png", IMREAD_GRAYSCALE);
*/