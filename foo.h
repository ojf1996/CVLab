#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include <vector>

#include <QImage>

#ifndef FOO_H
#define FOO_H

class Foo
{
public:
    //@remark 此函数用于openCV的Mat与QImage之间的转换
    //@param in是输入的mat
    //@param out是转换后的结果
    static void cvMatToQImage(const cv::Mat & in, QImage& out);

    //@remark 此函数用于openCV的Mat与QImage之间的转换
    //@param in是输入的QImage
    //@param out是Q转换后的结果
    static void QImageTocvMat(const QImage& in, cv::Mat& out);

    //@remark k=2的knnMatch，去除野点
    //@param in是输入的描述子
    //@param out是另一幅图像的描述子
    //@param matches是最终得到的匹配对
    static void _2nnMatch(cv::InputArray in,cv::InputArray out,std::vector<cv::DMatch>& matches);


    //@remark 随机抽样一致函数，为knnMatch之后的结果进行筛选得出较好的匹配
    //@param image01是第一幅图像
    //@param image02是另一幅图像的
    //@param out是输出结果
    //@param matches是得到的匹配对
    //@param keypoint1是第一张图提取的特征点
    //@param keypoint2是第二张图提取的特征点
    //@param ransacReprojThreshold是循环的阀值
    static void ransac(std::vector<cv::DMatch> &matches, std::vector<cv::KeyPoint> &keyPoint1,
                std::vector<cv::KeyPoint>& keyPoint2, std::vector<char>& matchesMask,int ransacReprojThreshold = 5);

    //@remark 用于画出图片关键点的梯度方向
    static void myDrawKeypoint(const cv::Mat& img, const std::vector<cv::KeyPoint>& keypoints,cv::Mat& out, const cv::Scalar& color =  cv::Scalar::all(-1));

    //@remark 用于画出真正匹配的点对
    static void myDrawMatches(const cv::Mat& in,const std::vector<cv::KeyPoint>& keypoints1,
                              const cv::Mat& in2,const std::vector<cv::KeyPoint>& keypoints2,
                              const std::vector<cv::DMatch>& matches1to2,cv::Mat& out,
                              const std::vector<char>& matchesMask,const cv::Scalar& matchColor = cv::Scalar::all((-1)));

    //@remark 通过图像配准实现拼接
    static void myStitch(const cv::Mat& left_img, const cv::Mat& right_img,
                         cv::Mat& dst,int method);
};


#endif // FOO_H
