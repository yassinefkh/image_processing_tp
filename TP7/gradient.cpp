#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat computeGradientMagnitude(const cv::Mat& image) {
    cv::Mat grad_x, grad_y, magnitude, magnitude_scaled;

    cv::Sobel(image, grad_x, CV_32F, 1, 0, 3);
    cv::Sobel(image, grad_y, CV_32F, 0, 1, 3);

    cv::magnitude(grad_x, grad_y, magnitude);

    cv::normalize(magnitude, magnitude_scaled, 0, 255, cv::NORM_MINMAX, CV_8U);

    return magnitude_scaled;
}

cv::Mat computeGradientMagnitudeBis(const cv::Mat& image){
    // without opencv functions
    cv::Mat grad_x, grad_y, magnitude;
    cv::Mat magnitude_scaled;
    cv::Mat grad_x_squared, grad_y_squared;

    cv::Mat kernel_x = (cv::Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat kernel_y = (cv::Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    cv::filter2D(image, grad_x, -1, kernel_x);
    cv::filter2D(image, grad_y, -1, kernel_y);

    cv::multiply(grad_x, grad_x, grad_x_squared);
    cv::multiply(grad_y, grad_y, grad_y_squared);

    cv::sqrt(grad_x_squared + grad_y_squared, magnitude);

    cv::normalize(magnitude, magnitude_scaled, 0, 255, cv::NORM_MINMAX, CV_8U);

    return magnitude_scaled;

}

cv::Mat thresholdImage(const cv::Mat& image, int thresholdValue) {
    cv::Mat thresholded;
    cv::threshold(image, thresholded, thresholdValue, 255, cv::THRESH_BINARY);
    return thresholded;
}

cv::Mat extractSeam(const cv::Mat& image) {
    cv::Mat morph_result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(image, morph_result, cv::MORPH_OPEN, kernel);
    
    return morph_result;
}

cv::Mat computeGradientOrientation(const cv::Mat& image) {
    cv::Mat grad_x, grad_y, angle;
    cv::Sobel(image, grad_x, CV_32F, 1, 0, 3);
    cv::Sobel(image, grad_y, CV_32F, 0, 1, 3);
    cv::phase(grad_x, grad_y, angle, true);
    cv::Mat angle_normalized;
    cv::normalize(angle, angle_normalized, 0, 255, cv::NORM_MINMAX, CV_8U);

    return angle_normalized;
}

int main() {
    cv::Mat image = cv::imread("7-images/football.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image." << std::endl;
        return -1;
    }

    cv::Mat gradient_magnitude = computeGradientMagnitude(image);
    cv::imwrite("gradient_magnitude.jpg", gradient_magnitude);
    cv::imshow("Gradient magnitude", gradient_magnitude);

    cv::Mat thresholded = thresholdImage(gradient_magnitude, 100);
    cv::imwrite("thresholded.jpg", thresholded);
    cv::imshow("Thresholded", thresholded);

    cv::Mat seam = extractSeam(thresholded);
    cv::imwrite("seam.jpg", seam);
    cv::imshow("Seam", seam);

    cv::Mat gradient_orientation = computeGradientOrientation(image);
    cv::applyColorMap(gradient_orientation, gradient_orientation, cv::COLORMAP_JET);
    cv::imwrite("gradient_orientation.jpg", gradient_orientation);
    cv::imshow("Gradient orientation : heatmap)", gradient_orientation);

    cv::waitKey(0);
    return 0;
}

