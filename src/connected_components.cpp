#include <opencv2/opencv.hpp>
#include <stack>

using namespace cv;
using namespace std;

vector<Point> getNeighbors4(Point p, int rows, int cols) {
    vector<Point> neighbors;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (abs(dx) == abs(dy)) continue; // exclut les diagonales et le pixel central
            int nx = p.x + dx, ny = p.y + dy; // nouvelle coord apres deplacement
            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                neighbors.push_back(Point(nx, ny));
            }
        }
    }
    return neighbors;
}


vector<Point> getNeighbors8(Point p, int rows, int cols) {
    vector<Point> neighbors;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; 
            int nx = p.x + dx, ny = p.y + dy;
            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                neighbors.push_back(Point(nx, ny));
            }
        }
    }
    return neighbors;
}

Mat ccLabel(Mat image) {
    Mat res = Mat::zeros(image.rows, image.cols, CV_32SC1); 
    int label = 1; 

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            // si pixel non nul et non encore labellisé
            if (image.at<uchar>(y, x) > 0 && res.at<int>(y, x) == 0) {
                // nouvelle composante détectée -> on lui assigne un nouveau label
                stack<Point> s;
                s.push(Point(x, y));
                res.at<int>(y, x) = label;

                // DFS
                while (!s.empty()) {
                    Point p = s.top();
                    s.pop();

                    for (Point neighbor : getNeighbors4(p, image.rows, image.cols)) {
                        if (image.at<uchar>(neighbor.y, neighbor.x) > 0 && res.at<int>(neighbor.y, neighbor.x) == 0) {
                            res.at<int>(neighbor.y, neighbor.x) = label;
                            s.push(neighbor);
                        }
                    }
                }
                label++; 
            }
        }
    }
    return res;
}

Mat visualizeLabels(const Mat& labels) {
    Mat colorImage(labels.size(), CV_8UC3);
    RNG rng;
    map<int, Vec3b> colorMap;

    for (int y = 0; y < labels.rows; y++) {
        for (int x = 0; x < labels.cols; x++) {
            int label = labels.at<int>(y, x);
            if (label > 0) {
                if (colorMap.find(label) == colorMap.end()) {
                    colorMap[label] = Vec3b(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
                }
                colorImage.at<Vec3b>(y, x) = colorMap[label];
            } else {
                colorImage.at<Vec3b>(y, x) = Vec3b(0, 0, 0); 
            }
        }
    }
    return colorImage;
}

cv::Mat ccAreaFilter(cv::Mat image, int size)
{
    Mat res = Mat::zeros(image.rows, image.cols, image.type());
    assert(size > 0);

    Mat labeled = ccLabel(image);

    map<int, int> componentSize; // map label->nb de pixels

    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            int label = labeled.at<int>(y, x);
            if (label > 0) {
                componentSize[label]++;
            }
        }
    }

    for (int y = 0; y < labeled.rows; y++) {
        for (int x = 0; x < labeled.cols; x++) {
            int label = labeled.at<int>(y, x);
            if (label > 0 && componentSize[label] >= size) {
                res.at<uchar>(y, x) = 255; 
            }
        }
    }

    return res;
}

cv::Mat ccTwoPassLabel(cv::Mat image)
{
    Mat res = Mat::zeros(image.rows, image.cols, CV_32SC1);
    int rows = image.rows, cols = image.cols;
    int labelCounter = 1; 
    map<int, int> labelEquivalence; 

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (image.at<uchar>(y, x) > 0) { 
                vector<int> neighbors;

                if (x > 0 && res.at<int>(y, x - 1) > 0) 
                    neighbors.push_back(res.at<int>(y, x - 1));
                if (y > 0 && res.at<int>(y - 1, x) > 0)
                    neighbors.push_back(res.at<int>(y - 1, x));

                if (neighbors.empty()) {
                    res.at<int>(y, x) = labelCounter;
                    labelEquivalence[labelCounter] = labelCounter; 
                    labelCounter++;
                } else {
                    int minLabel = *min_element(neighbors.begin(), neighbors.end());
                    res.at<int>(y, x) = minLabel;

                    for (int lbl : neighbors) {
                        if (lbl != minLabel) {
                            labelEquivalence[lbl] = minLabel; 
                        }
                    }
                }
            }
        }
    }


    for (auto& entry : labelEquivalence) {
        int label = entry.first;
        while (labelEquivalence[label] != label) { 
            label = labelEquivalence[label];
        }
        entry.second = label;
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (res.at<int>(y, x) > 0) {
                res.at<int>(y, x) = labelEquivalence[res.at<int>(y, x)];
            }
        }
    }

    return res;
}



int main(){
    // !!! Pensez à charger votre image ici !!! 
    cv::Mat image = cv::imread("", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Erreur : Impossible de charger l'image." << std::endl;
        return -1;
    }


    // --- Analyse en composante connexe --- //
    Mat labeled = ccLabel(image);
    Mat colored = visualizeLabels(labeled);

    Mat twoPassLabeled = ccTwoPassLabel(image);
    Mat twoPassColored = visualizeLabels(twoPassLabeled);

    // --- Filtre d'air --- //
    Mat filteredImage = ccAreaFilter(image, 200);
    imshow("Filtered image", filteredImage);


    Mat displayRes;
    normalize(labeled, displayRes, 0, 255, NORM_MINMAX, CV_8U);
    Mat displayResBis;
    normalize(twoPassLabeled, displayResBis, 0, 255, NORM_MINMAX, CV_8U);

    imshow("CC with ccLabel", twoPassColored);
    imshow("CC with ccTwoPassLabel", twoPassColored);
    imshow("Filtered image", filteredImage);




    waitKey(0);

    return 0;
}