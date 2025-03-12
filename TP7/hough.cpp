#include <opencv2/opencv.hpp>

cv::Mat detectLines(const cv::Mat& image) {
    cv::Mat edges, result;
    cv::Canny(image, edges, 50, 150);
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(edges, lines, 1, CV_PI / 180, 150);
    result = image.clone();
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 100 * (-b));
        pt1.y = cvRound(y0 + 100 * (a));
        pt2.x = cvRound(x0 - 100 * (-b));
        pt2.y = cvRound(y0 - 100 * (a));
        cv::line(result, pt1, pt2, cv::Scalar(0, 0, 255), 2);
    }
    return result;
}

cv::Mat detectCircles(const cv::Mat& image) {
    cv::Mat gray, blurred, result;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(9, 9), 2, 2);
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(blurred, circles, cv::HOUGH_GRADIENT, 1, blurred.rows / 8, 200, 50, 0, 0);
    result = image.clone();
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(result, center, radius, cv::Scalar(0, 255, 0), 2);
    }
    return result;
}

int main() {
    cv::Mat corridor = cv::imread("7-images/corridor.png", cv::IMREAD_GRAYSCALE);
    cv::Mat coins = cv::imread("7-images/coins.png");

    if (corridor.empty() || coins.empty()) return -1;

    cv::Mat linesDetected = detectLines(corridor);
    cv::Mat circlesDetected = detectCircles(coins);

    cv::imwrite("corridor_lines.png", linesDetected);
    cv::imwrite("coins_circles.png", circlesDetected);

    cv::imshow("Lines", linesDetected);
    cv::imshow("Circles", circlesDetected);

    cv::waitKey(0);
    return 0;
}
