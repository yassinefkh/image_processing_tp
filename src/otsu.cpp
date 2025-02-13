#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int calculateThresholdOtsu(const Mat& image) {
    double min_val, max_val;
    minMaxLoc(image, &min_val, &max_val);

    int best_threshold = 0;
    double max_variance = 0;

    for (int threshold = (int)min_val; threshold <= (int)max_val; threshold++) { // on test tous les seuils possible entre la val min et max
        Mat class_1 = image < threshold;
        Mat class_2 = image >= threshold;

        int count_1 = countNonZero(class_1);
        int count_2 = countNonZero(class_2);
        std::cout << "Nombre de pixels dans la classe 1: " << count_1 << std::endl;
        std::cout << "Nombre de pixels dans la classe 2: " << count_2 << std::endl;

        if (count_1 == 0 || count_2 == 0) continue;

        // moyenne des pixels dans chaque classe
        Scalar mean_1, mean_2;
        meanStdDev(image, mean_1, noArray(), class_1);
        meanStdDev(image, mean_2, noArray(), class_2);

        // proportion des pixels dans chaque classe
        double weight_1 = (double)count_1 / (image.rows * image.cols);
        double weight_2 = (double)count_2 / (image.rows * image.cols);
        std::cout << "weight_1 = " << weight_1 << std::endl;
        std::cout <<"weight_2 = " << weight_2 << std::endl;

        // var inter classe: plus elle est grande, plus il y a une separation nette
        double variance_between = weight_1 * weight_2 * pow(mean_1[0] - mean_2[0], 2);
        std::cout << "Variance inter-classe = " << variance_between << std::endl;

        if (variance_between > max_variance) {
            max_variance = variance_between;
            std::cout << "max variance = " << max_variance << std::endl;
            best_threshold = threshold;
        }
    }

    return best_threshold;
}

Mat thresholdOtsu(Mat image) {
    Mat res = image.clone();

    int best_threshold = calculateThresholdOtsu(image);

    threshold(res, res, best_threshold, 255, THRESH_BINARY);

    cout << "Meilleur seuil trouvé : " << best_threshold << endl;
    return res;
}

int main() {
    // !!! Pensez à charger votre image ici !!! 
    Mat image = imread("", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Erreur : Impossible de charger l'image." << endl;
        return -1;
    }
    Mat thresholded_image = thresholdOtsu(image);
    imshow("Image originale", image);
    imshow("Image seuillee", thresholded_image);
    waitKey(0);
    return 0;
}
