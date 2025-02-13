#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>

using namespace cv;
using namespace std;

const int SEUIL = 20; // seuil d'homogénéité
Mat image, segmented; 

Mat regionGrowing(Mat image, Point seed) {
    Mat regions = Mat::zeros(image.size(), CV_8U); 
    queue<Point> S;
    S.push(seed);

    int sum = image.at<uchar>(seed);
    int count = 1; 
    int mean = sum; 

    // 4-connexité
    vector<Point> neighbors = {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};

    while (!S.empty()) {
        Point p = S.front();
        S.pop();

        for (Point d : neighbors) {
            Point Pn = p + d;
            if (Pn.x >= 0 && Pn.x < image.cols && Pn.y >= 0 && Pn.y < image.rows) {
                if (regions.at<uchar>(Pn) == 0) { 
                    int intensity = image.at<uchar>(Pn);
                    int distance = abs(intensity - mean);

                    if (distance <= SEUIL) { 
                        regions.at<uchar>(Pn) = 255; 
                        S.push(Pn); 
                        sum += intensity;
                        count++;
                        mean = sum / count; 
                    }
                }
            }
        }
    }

    return regions;
}

void onMouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) { 
        cout << "Point sélectionné : (" << x << ", " << y << ")" << endl;
        segmented = regionGrowing(image, Point(x, y));
        imshow("Region growing segmentation", segmented);
    }
}

int main() {
    // !!! Pensez à charger votre image ici !!! 
    image = imread("", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Erreur : Impossible de charger l'image." << endl;
        return -1;
    }

    namedWindow("Image originale"); // afficher l'image originale et attend un click de souris
    setMouseCallback("Image originale", onMouse);

    imshow("Image originale", image);
    waitKey(0); 

    return 0;
}
