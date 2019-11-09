#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int main( int argc, char* argv[] )
{

    double t = (double)getTickCount();

    cv::VideoCapture capture("cars2.mp4"); // input video present in input folder

    double fps = capture.get( cv::CAP_PROP_FPS );// obtaining  frames per second

    cv::Size size(
                  (int)capture.get( cv::CAP_PROP_FRAME_WIDTH ),
                  (int)capture.get( cv::CAP_PROP_FRAME_HEIGHT )
                  );// getting width and height of video


    cv::Mat bgr_frame1,frame_gray1, img_keypoints,bgr_frame2, frame_gray2,gray2, img_keypoints2, bgr_frame3,frame_gray3,img_keypoints3;

    std::vector<KeyPoint> keypoints1 , keypoints2, keypoints3;

    cv::Size winSize = Size(51,51);

    int maxLevel = 5;  // Specify the number of levels of pyramidal abstraction
    double minEigThreshold = 1e-2; // Specify the Eigen value thresholds

    cv::TermCriteria criteria= TermCriteria(cv::TermCriteria::EPS,10000,0.59);

    Mat descriptors, err;

    vector<uchar> status;

    CV_OUT std::vector<Point2f> points2f1, points2f2, points2f3;

	// vector < Scalar > color;

    int length = int(capture.get(CAP_PROP_FRAME_COUNT));

    const std::vector< int > & 	keypointIndexes = std::vector< int >();

    const std::vector< int > & 	keypointIndexes2 = std::vector< int >();

    Ptr<FastFeatureDetector> Detector = FastFeatureDetector::create(30, true, FastFeatureDetector::TYPE_7_12);
    		//Define an instance for FAST detector, we set window size to 7 by 12

    vector<Scalar> color;

    cv::VideoWriter writer; // video writer object

    capture >> bgr_frame1; // taking first frame from the video and storing in bgr_frame1

    cv::cvtColor( bgr_frame1, frame_gray1, cv::COLOR_BGR2GRAY);// converting bgr_frame1 to gray image and storing in frame_gray1


    Detector -> detect(frame_gray1, keypoints1);// detecting keypoints for first frame using FAST algorithm

    KeyPoint::convert(keypoints1,points2f1,keypointIndexes);// converting keypoints of first frame to Points2f type

    Mat imgLines = Mat::zeros( frame_gray1.size(), CV_8UC3 );// creating an empty image with size equals to frame_gray1 size.
    // char c = cv::waitKey(fps);

    capture >> bgr_frame2;// get second frame from the video

    cv::cvtColor( bgr_frame2, frame_gray2, cv::COLOR_BGR2GRAY);//converting bgr_frame2 to gray image and storing in frame_gray2

    Detector->detect(frame_gray2, keypoints2);// detecting keypoints for second frame using the FAST algorithm

    KeyPoint::convert(keypoints2,points2f2,keypointIndexes2);//converting keypoints of 2nd frame to Points2f type

    length = length -1;

    writer.open("outputLK.mp4", VideoWriter::fourcc('M','J','P','G'), fps, size );// output video path and type

    for (signed int j = 0; j < 5 * points2f2.size(); j++)
    {
        color.push_back(Scalar(rand() % 255, rand() % 255, rand() % 255));
    }

    for(;;) // infinite loop

    {
        length = length -1;
        if (length == 2 )
            break;
        calcOpticalFlowPyrLK(frame_gray1,frame_gray2, points2f1,points2f2,status,err,winSize,maxLevel,criteria,cv::OPTFLOW_LK_GET_MIN_EIGENVALS, minEigThreshold); //calculating optical flow for two frames
        // Using Lucas Kanade Optical flow for Sparse optical flow estimation

        frame_gray2.copyTo(frame_gray1); // copying content of frame_gray2 to frame_gray1

        // Define different lines for each feature point tracked between frames
        for(size_t i=0; i<points2f2.size(); i++)//
        {
            if(status[i])
            {
            	if ( i != 0 )
				{
					arrowedLine(imgLines,points2f1[i],points2f2[i],color[i],1,8,0,0.5);
				}

                points2f1[i].x = points2f2[i].x;
                points2f1[i].y = points2f2[i].y;
            }

        }

        bgr_frame2 = bgr_frame2 + imgLines; // adding the original image and the image with lines


        imshow("Tracked using FAST & L-K pyramid method",bgr_frame2); // displaying the image after addition

        writer << bgr_frame2; // saving bgr_frame2 to writer
        //    if( c == 27)  break;

        cv::waitKey(fps);

        capture >> bgr_frame2;   // capturing a new frame from the input video

        cv::cvtColor( bgr_frame2, frame_gray2, cv::COLOR_BGR2GRAY); //c onverting bgr_frame2 to gray image and storing in frame_gray2

        Detector->detect(frame_gray2, keypoints2);// detecting keypoints for frame_gray2 using ORB algorithm

        KeyPoint::convert(keypoints2,points2f2,keypointIndexes2);//converting keypoints2 to Points2f type
        //   if( c == 27)  break;
    }
        t = ((double)getTickCount() - t)/getTickFrequency();

        std::cout << "Times passed in seconds: " << t << std::endl;


    return 0;

}
