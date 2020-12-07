#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>


std::string GetName(std::string def = "Your name here") {
  bool do_debug = false;
  std::string cur_str = def;

  // create window
  std::string window_name = "Enter your name";
  cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

  for (;;) {
    // create image
    cv::Mat frame(cv::Size(500, 100), CV_64FC1);
    frame = cv::Scalar(255, 255, 255);
    cv::putText(frame, cur_str.c_str(), cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0));

    // get input
    char ch = cv::waitKey(1);
    if (do_debug) std::cout << ch << "-" << static_cast<int>(ch) << std::endl;

    // analyse char
    if (ch == 8) {
      if (cur_str.size() > 0) {
        cur_str.erase(cur_str.end() - 1);
      }
    } else if (ch == 13) {
      return cur_str;
    } else if (ch == -1) {

    } else {
      if (cur_str == def) {
        cur_str = "";
        cur_str += ch;
      } else {
        cur_str += ch;
      }
    }

    cv::imshow(window_name, frame); 
  }
  return def;
}

int main() {
  std::cout << GetName();
  ;
}
