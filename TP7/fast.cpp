#include <opencv2/opencv.hpp>

cv::Mat detectFASTKeypoints(const cv::Mat& image) {
    cv::Mat gray, result;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::FastFeatureDetector> fast = cv::FastFeatureDetector::create(50);
    fast->detect(gray, keypoints);
    result = image.clone();
    cv::drawKeypoints(image, keypoints, result, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    return result;
}

int main() {
    cv::Mat image = cv::imread("7-images/bretelle.jpg");
    if (image.empty()) return -1;
    cv::Mat fastKeypoints = detectFASTKeypoints(image);
    cv::imwrite("fast_keypoints.jpg", fastKeypoints);
    cv::imshow("FAST Keypoints", fastKeypoints);
    cv::waitKey(0);
    return 0;
}
