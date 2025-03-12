#include <opencv2/opencv.hpp>

cv::Mat detectLines(const cv::Mat& image) {
    cv::Mat gray, edges, result;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 50, 150, 3);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 80, 100, 3);

    result = image.clone(); 

    for (const auto& line : lines) {
        cv::line(result, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), 
                 cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    }

    return result;
}

cv::Mat detectCircles(const cv::Mat& image) {
    cv::Mat gray, blurred, result;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(9, 9), 2, 2);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, blurred.rows / 8, 200, 20, 0, 0);

    result = image.clone();
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(result, center, radius, cv::Scalar(0, 255, 0), 2);
    }

    return result;
}

int main() {
    cv::Mat corridor = cv::imread("7-images/corridor.png", cv::IMREAD_COLOR); 
    cv::Mat coins = cv::imread("7-images/coins.png", cv::IMREAD_COLOR);

    if (corridor.empty() || coins.empty()) return -1;

    cv::Mat linesDetected = detectLines(corridor);
    cv::Mat circlesDetected = detectCircles(coins);
   
    cv::imwrite("corridor_lines.png", linesDetected);
    cv::imwrite("coins_circles.png", circlesDetected);

    cv::imshow("Original corridor", corridor);
    cv::imshow("Original coins", coins);
    cv::imshow("Lines", linesDetected);
    cv::imshow("Circles", circlesDetected);

    cv::waitKey(0);
    return 0;
}
