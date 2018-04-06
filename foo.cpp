#include "foo.h"
#include <QDebug>

//------------------------------------------------------
static void _drawKeypoint(cv::Mat& img,const cv::KeyPoint&p, const cv::Scalar& color)
{
    const int draw_shift_bits = 4;
    const int draw_multiplier = 1 << draw_shift_bits;

    cv::Point center( cvRound(p.pt.x * draw_multiplier), cvRound(p.pt.y * draw_multiplier));

    int radius = cvRound(p.size/2 * draw_multiplier);

    if(p.angle != 1)
    {
        float srcAngleRad = p.angle*(float)CV_PI/180.f;
        cv::Point orient( cvRound(cos(srcAngleRad)*radius ),
                      cvRound(sin(srcAngleRad)*radius )
                     );
        cv::line( img, center, center+orient, color, 1, cv::LINE_AA, draw_shift_bits );
    }
    else
    {
        int radius = 1 * draw_multiplier;
        cv::circle( img, center, radius, color, 1,cv::LINE_AA,draw_shift_bits);
    }
}


//-------------------------------------------------------
void Foo::cvMatToQImage(const cv::Mat & in, QImage &out)
{
    switch(in.type())
       {
           //RGBA
           case CV_8UC4:
           {
               cv::Mat view(in);
               QImage view2(view.data, view.cols, view.rows, view.step[0], QImage::Format_ARGB32);
               out = view2.copy();
               break;
           }
           //BGR2RGB
           case CV_8UC3:
           {
               cv::Mat mat;
               cvtColor(in, mat, cv::COLOR_BGR2BGRA); //COLOR_BGR2RGB doesn't behave so use RGBA
               QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
               out = view.copy();
               break;
           }
           //灰度图
           case CV_8UC1:
           {
               cv::Mat mat;
               cv::cvtColor(in, mat, cv::COLOR_GRAY2BGRA);
               QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
               out = view.copy();
               break;
           }
           default:
           {
               out = QImage();
               return;
           }
       }
}
//------------------------------------------------------------
void Foo::QImageTocvMat(const QImage &in, cv::Mat &out)
{
    switch(in.format()) {
           case QImage::Format_Invalid:
           {
               cv::Mat empty;
               empty.copyTo(out);
               break;
           }
           case QImage::Format_Indexed8:
           {
                cv::Mat view(in.height(),in.width(),CV_8UC1,(void *)in.constBits(),in.bytesPerLine());
                view.copyTo(out);
                break;
           }
           case QImage::Format_RGB32:
           {
               cv::Mat view(in.height(),in.width(),CV_8UC4,(void *)in.constBits(),in.bytesPerLine());
               view.copyTo(out);
               break;
           }
           case QImage::Format_RGB888:
           {
               cv::Mat view(in.height(),in.width(),CV_8UC3,(void *)in.constBits(),in.bytesPerLine());
               cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
               break;
           }
           default:
           {
               QImage conv = in.convertToFormat(QImage::Format_ARGB32);
               cv::Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
               view.copyTo(out);
               break;
           }
       }
}

//-------------------------------------------------------------
void Foo::_2nnMatch(cv::InputArray queryDescriptors, cv::InputArray trainDescriptors,std::vector<cv::DMatch>& matches)
{
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
    std::vector<std::vector<cv::DMatch>> knnMatches;
    matcher->knnMatch(queryDescriptors,trainDescriptors,knnMatches,2);

    matches.clear();
    matches.reserve(knnMatches.size());
    //采用0.8作为阀值 1NN / 2NN < 0.8
    const float minRatio = 1.f / 1.5f;
    for(size_t i = 0; i < knnMatches.size(); i++)
    {
        const cv::DMatch& closestMatch = knnMatches[i][0];
        const cv::DMatch& secondClosestMatch = knnMatches[i][1];

        float distanceRatio = closestMatch.distance / secondClosestMatch.distance;

        //只有当比值小于minRatio才认为是好的匹配
        if(distanceRatio < minRatio){
            matches.push_back(closestMatch);
        }

    }
}
//--------------------------------------------------------------
void Foo::ransac(cv::Mat &image01, cv::Mat &image02, cv::Mat &out,
                 std::vector<cv::DMatch> &matches, std::vector<cv::KeyPoint> &keyPoint1,
                 std::vector<cv::KeyPoint> &keyPoint2, int ransacReprojThreshold)
{
        std::vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
        for (size_t i = 0; i < matches.size(); i++)
        {
            queryIdxs[i] = matches[i].queryIdx;
            trainIdxs[i] = matches[i].trainIdx;
        }

        cv::Mat H12;   //海森矩阵

        std::vector<cv::Point2f> points1;
        cv::KeyPoint::convert(keyPoint1, points1, queryIdxs);
        std::vector<cv::Point2f> points2;
        cv::KeyPoint::convert(keyPoint2, points2, trainIdxs);


        H12 = cv::findHomography(cv::Mat(points1), cv::Mat(points2), CV_RANSAC, ransacReprojThreshold);
        std::vector<char> matchesMask(matches.size(), 0);
        cv::Mat points1t;
        cv::perspectiveTransform(cv::Mat(points1), points1t, H12);
        for (size_t i1 = 0; i1 < points1.size(); i1++)  //保存inliers
        {
            if (norm(points2[i1] - points1t.at<cv::Point2f>((int)i1, 0)) <= ransacReprojThreshold) //给内点做标记
            {
                matchesMask[i1] = 1;
            }
        }
        cv::Mat match_img2;   //滤除outliers
        cv::drawMatches(image01, keyPoint1, image02, keyPoint2, matches, match_img2, cv::Scalar(0, 0, 255), cv::Scalar::all(-1), matchesMask);
        out = match_img2.clone();
}
//---------------------------------------------------------------------
void Foo::myDrawKeypoint(const cv::Mat& img, const std::vector<cv::KeyPoint>& keypoints,cv::Mat& outImage, const cv::Scalar& _color)
{
    //先将原图
    int imgtype = img.type(),imgcn = CV_MAT_CN(imgtype);
    if(imgcn != 1)
        cv::cvtColor(img,outImage,CV_BGR2GRAY);
    else
        img.copyTo(outImage);

    //设置颜色
    bool isDIYColor = _color == cv::Scalar::all(-1);

    std::vector<cv::KeyPoint>::const_iterator it = keypoints.begin(),
                                         end = keypoints.end();
    for( ; it != end; ++it )
    {
        cv::Scalar color = isDIYColor ? cv::Scalar(255,255,255) : _color;
        _drawKeypoint( outImage, *it, color);
    }
}
