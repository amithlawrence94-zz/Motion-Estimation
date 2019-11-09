#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

const float GOOD_MATCH_PERCENT = 0.38f;


void alignImages(Mat &im1, Mat &im2, Mat &im1Reg, Mat &h)

{

  // Convert images to grayscale

  Mat im1Gray, im2Gray;

  cvtColor(im1, im1Gray, COLOR_RGB2GRAY);
  cvtColor(im2, im2Gray, COLOR_RGB2GRAY);

  // Variables to store keypoints and descriptors

  std::vector<KeyPoint> keypoints1, keypoints2;

  Mat descriptors1, descriptors2;

  Mat img_keypoints_1, img_keypoints_2;

  // Detect FAST features

  //--Step 1: Detect the keypoints using a FAST Detector

  Ptr<FastFeatureDetector> Detector = FastFeatureDetector::create(30, false, FastFeatureDetector::TYPE_9_16);

  Detector->detect(im1Gray, keypoints1);
  Detector->detect(im2Gray, keypoints2);

  drawKeypoints(im1Gray, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  drawKeypoints(im2Gray, keypoints2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  imshow("Keypoints 1", img_keypoints_1);
  imshow("Keypoints 2", img_keypoints_2);

  // Draw the feature descriptors from the detected keypoints

  Ptr<FREAK> Descriptor = FREAK::create(true, true);

  Descriptor->compute(im1Gray, keypoints1, descriptors1);
  Descriptor->compute(im2Gray, keypoints2, descriptors2);

  // Match features.

  std::vector<DMatch> matches;

  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

  matcher->match(descriptors1, descriptors2, matches, Mat());

  // Sort matches by score
  std::sort(matches.begin(), matches.end());

  // Remove not so good matches
  const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
  matches.erase(matches.begin()+numGoodMatches, matches.end());


  // Draw top matches
  Mat imMatches;
  drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);

  imshow("Matched Descriptors", imMatches);

  waitKey();

  imwrite("matches.jpg", imMatches);


  // Extract location of good matches
  std::vector<Point2f> points1, points2;

  for( size_t i = 0; i < matches.size(); i++ )
  {
    points1.push_back( keypoints1[ matches[i].queryIdx ].pt );
    points2.push_back( keypoints2[ matches[i].trainIdx ].pt );
  }

/*  // Find homography
  h = findHomography( points1, points2, RANSAC );

  // Use homography to warp image
  warpPerspective(im1, im1Reg, h, im2.size());*/

}


int main(int argc, char **argv)
{
  // Read reference image
  string refFilename("Quad.jpg");
  cout << "Reading reference image : " << refFilename << endl;
  Mat imReference = imread(refFilename);


  // Read image to be aligned
  string imFilename("T_Quad.jpg");
  cout << "Reading image to align : " << imFilename << endl;
  Mat im = imread(imFilename);


  // Registered image will be resotred in imReg.
  // The estimated homography will be stored in h.
  Mat imReg, h;

  // Align images
  cout << "Aligning images ..." << endl;
  alignImages(im, imReference, imReg, h);

  // Write aligned image to disk.
  string outFilename("aligned.jpg");
  cout << "Saving aligned image : " << outFilename << endl;
  imwrite(outFilename, imReg);

  // Print estimated homography
  cout << "Estimated homography : \n" << h << endl;

}
