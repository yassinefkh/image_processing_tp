#include <opencv2/opencv.hpp>

cv::Mat detectHarrisCorners(const cv::Mat& image) {
    cv::Mat gray, corners, result;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::cornerHarris(gray, corners, 2, 3, 0.04);
    cv::normalize(corners, corners, 0, 255, cv::NORM_MINMAX, CV_8U);
    result = image.clone();
    for (int i = 0; i < corners.rows; i++) {
        for (int j = 0; j < corners.cols; j++) {
            if (corners.at<uchar>(i, j) > 150) {
                cv::circle(result, cv::Point(j, i), 3, cv::Scalar(0, 0, 255), -1);
            }
        }
    }
    return result;
}

cv::Mat rotateImage(const cv::Mat& image, double angle) {
    cv::Mat rotated;
    cv::Point2f center(image.cols / 2.0, image.rows / 2.0);
    cv::Mat rotMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(image, rotated, rotMatrix, image.size());
    return rotated;
}

int main() {
    cv::Mat image = cv::imread("7-images/bretelle.jpg");
    if (image.empty()) return -1;

    cv::Mat cornersDetected = detectHarrisCorners(image);
    cv::imwrite("harris_corners.jpg", cornersDetected);
    cv::imshow("Harris corners", cornersDetected);

    cv::Mat rotatedImage = rotateImage(image, 45);
    cv::Mat rotatedCorners = detectHarrisCorners(rotatedImage);
    cv::imwrite("harris_corners_rotated.jpg", rotatedCorners);
    cv::imshow("Harris corners rotated", rotatedCorners);

    cv::waitKey(0);
    return 0;
}
