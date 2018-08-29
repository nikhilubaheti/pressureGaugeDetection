#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


vector<Vec3f> detectCircles(const Mat src)
{
    Mat src_gray;

    vector<Vec3f> circles;

    /// Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    /// Apply the Hough Transform to find the circles
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 100, 200, 0, 0 );

    return circles;
}

Vec2i detectNeedle(const Mat src, const Vec3f circle)
{
    Mat src_canny;

    Vec2i NeedlePoint(-1,-1);
    vector<Vec4i> lines;

    Point center(cvRound(circle[0]), cvRound(circle[1]));
    int radius = cvRound(circle[2]);
    bool leftCenterLine = false, RightCenterLine = false, isNeedle = false;

    Vec3f equationOfLeftLine, equationOfRightLine;

    /// Line detection
    Canny(src, src_canny, 50, 100, 3);

    HoughLinesP(src_canny, lines, 1, CV_PI/180, 50, radius/2, radius/10);

    /// Check if line is around center by projecting point on line
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        Vec3f equationOfLine(l[3] - l[1],l[0] - l[2],(l[3] - l[1])*(-l[2]) + (l[0] - l[2])*(-l[3]) );
        Vec3f centerHomogenous(center.x,center.y,1);
        if( abs(equationOfLine.dot(centerHomogenous)/sqrt(equationOfLine[0]*equationOfLine[0] + equationOfLine[1]*equationOfLine[1])) < (radius/10))
        {
            Vec2f p1(center.x - l[0], center.y - l[1]), p2(l[2] - l[0], l[3] - l[1]);
            Vec2f p2norm = p2/norm(p2);
            float p1dotp2norm = p1.dot(p2norm);
            if(( p1dotp2norm < (norm(p2))) && (p1dotp2norm > 0))
            {
                if(((equationOfLine.dot(centerHomogenous)) < 0) && !leftCenterLine )
                {
                    leftCenterLine = true;
                    equationOfLeftLine = equationOfLine;
                }
                if(((equationOfLine.dot(centerHomogenous)) > 0) && !RightCenterLine )
                {
                    RightCenterLine = true;
                    equationOfRightLine = equationOfLine;
                }
            }
        }
    }

    isNeedle = leftCenterLine && RightCenterLine;

    if(isNeedle)
    {
        ///Find intersection of Needle
        Mat tempLHS = (Mat_<float>(2,2) << equationOfLeftLine[0], equationOfLeftLine[1], equationOfRightLine[0], equationOfRightLine[1] );
        Mat tempRHS = (Mat_<float>(2,1) << -equationOfLeftLine[2], -equationOfRightLine[2] );
        Mat intersectionPoint = tempLHS.inv()*tempRHS;
        NeedlePoint = Vec2i(intersectionPoint.at<float>(Point(0, 0)),intersectionPoint.at<float>(1,0));


        std::cout << "slope: " << (float(NeedlePoint[1] - center.y)/(NeedlePoint[0] - center.x)) << std::endl;
    }

    return  NeedlePoint;
}


/** @function main */
int main(int argc, char** argv)
{
    Mat src;

  /// Read the image
  src = imread(  "../pressureGaugeDetection/pressureGuage4.jpg", CV_LOAD_IMAGE_COLOR );

  if( !src.data )
    { return -1; }

  vector<Vec3f> circles = detectCircles(src);

  Vec3f equationOfLeftLine, equationOfRightLine;

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);

      // circle center
      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );

      Vec2i needle = detectNeedle(src,circles[i]);
      if(needle[0] != -1)
      {
          line( src, center, Point(needle[0],needle[1]), Scalar(0,255,0), 3, CV_AA);
      }
  }
  imshow("source", src);

  waitKey(0);
  return 0;
}
