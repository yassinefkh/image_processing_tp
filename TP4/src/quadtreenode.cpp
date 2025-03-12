#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

const int SEUIL_HOMOGENE = 20;
const int SEUIL_SIMILARITE = 15;
bool affichageTempsReel = false;

class QuadTreeNode {
public:
    Rect region;
    bool isLeaf;
    Scalar meanColor;
    vector<QuadTreeNode*> children;

    QuadTreeNode(Rect r, Mat& image) : region(r), isLeaf(true) {
        Mat roi = image(region);
        Scalar mean, stddev;
        meanStdDev(roi, mean, stddev);
        meanColor = mean;
        isLeaf = (stddev[0] < SEUIL_HOMOGENE);
    }

    ~QuadTreeNode() {
        for (auto child : children) delete child;
    }

    bool isHomogeneous() {
        return isLeaf;
    }
};

void drawQuadTree(Mat& display, QuadTreeNode* node) {
    if (node->isLeaf) {
        rectangle(display, node->region, Scalar(255, 255, 255), 1);
        if (affichageTempsReel) {
            imshow("Segmentation en cours...", display);
            waitKey(100);
        }
        return;
    }
    for (auto& child : node->children) {
        drawQuadTree(display, child);
    }
}

void split(QuadTreeNode* node, Mat& image, Mat& display) {
    if (node->isHomogeneous()) return;

    int halfWidth = node->region.width / 2;
    int halfHeight = node->region.height / 2;

    if (halfWidth < 1 || halfHeight < 1) return;

    node->children.push_back(new QuadTreeNode(Rect(node->region.x, node->region.y, halfWidth, halfHeight), image));
    node->children.push_back(new QuadTreeNode(Rect(node->region.x + halfWidth, node->region.y, halfWidth, halfHeight), image));
    node->children.push_back(new QuadTreeNode(Rect(node->region.x, node->region.y + halfHeight, halfWidth, halfHeight), image));
    node->children.push_back(new QuadTreeNode(Rect(node->region.x + halfWidth, node->region.y + halfHeight, halfWidth, halfHeight), image));

    node->isLeaf = false;

    drawQuadTree(display, node);
    
    for (auto& child : node->children) {
        split(child, image, display);
    }
}


void merge(QuadTreeNode* node, Mat& segmented) {
    if (node->isLeaf) {
        rectangle(segmented, node->region, node->meanColor[0], FILLED);
        if (affichageTempsReel) {
            imshow("Segmentation en cours", segmented);
            waitKey(100);
        }
        return;
    }
    for (auto& child : node->children) {
        merge(child, segmented);
    }
}

int main() {
    // !!! Pensez à charger votre image ici !!! 
    Mat image = imread("/Volumes/SSD/M1VMI/S2/image_processing/ImageProcessingLab-main/bin/camera.png", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Erreur : Impossible de charger l'image." << endl;
        return -1;
    }

    affichageTempsReel = false;

    imshow("Image originale", image);

    Mat display = image.clone();
    cvtColor(display, display, COLOR_GRAY2BGR);

    QuadTreeNode* root = new QuadTreeNode(Rect(0, 0, image.cols, image.rows), image);
    split(root, image, display);

    Mat segmented = Mat::zeros(image.size(), CV_8U);
    merge(root, segmented);

    imshow("Segmentation finale", segmented);
    waitKey(0);

    delete root;
    return 0;
}
