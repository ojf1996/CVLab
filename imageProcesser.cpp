#include <QImage>
#include "imageProcesser.h"
#include "foo.h"
#include <QDebug>

//---------------------------------------
imageProcesser::imageProcesser(QObject *parent):
        QObject(parent)
{
}
//--------------------------------------------------------------------------------
void imageProcesser::matchTest(std::string input1,std::string input2,int type, bool is2NN)
{
    cv::Mat in1,in2,out,_in1,_in2;
    QImage matchImg,in1Img,in2Img;
    //由于QImage转cv：：Mat后，虽然可以显示，可是存在一些问题，故直接读取算了
    in1 = cv::imread(input1,cv::IMREAD_UNCHANGED);
    in2 = cv::imread(input2,cv::IMREAD_UNCHANGED);

    std::vector<cv::KeyPoint> key1, key2;
    cv::Mat desc1,desc2;

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    cv::Ptr<cv::xfeatures2d::SIFT> sift = cv::xfeatures2d::SIFT::create();
    cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create();

    switch (type) {
        case 1:
            orb->detectAndCompute(in1,cv::Mat(),key1,desc1);
            orb->detectAndCompute(in2,cv::Mat(),key2,desc2);
            break;
        case 2:
            sift->detectAndCompute(in1,cv::Mat(),key1,desc1);
            sift->detectAndCompute(in2,cv::Mat(),key2,desc2);
            break;
        case 3:
            surf->detectAndCompute(in1,cv::Mat(),key1,desc1);
            surf->detectAndCompute(in2,cv::Mat(),key2,desc2);
            break;
        default:
            emit finishMatch(QImage());
            return;
    }

    Foo::myDrawKeypoint(in1,key1,_in1);
    Foo::cvMatToQImage(_in1,in1Img);
    emit finishDetectPhoto1(in1Img);

    Foo::myDrawKeypoint(in2,key2,_in2);
    Foo::cvMatToQImage(_in2,in2Img);
    emit finishDetectPhoto2(in2Img);

    std::vector<cv::DMatch> matches;
    if(is2NN)
    {
        Foo::_2nnMatch(desc1,desc2,matches);
    }
    else
    {
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
        matcher->match(desc1,desc2,matches);
    }

    std::vector<char> matchesMask;

    Foo::ransac(matches,key1,key2,matchesMask);

    Foo::myDrawMatches(in1,key1,in2,key2,matches,out,matchesMask);

    Foo::cvMatToQImage(out,matchImg);

    emit finishMatch(matchImg);
}

void imageProcesser::stitchTest(std::string input1, std::string input2, int kind)
{
    cv::Mat in1,in2,out;
    QImage stitchImg;

    in1 = cv::imread(input1,-1);
    in2 = cv::imread(input2,-1);

    Foo::myStitch(in1,in2,out,kind);

    Foo::cvMatToQImage(out,stitchImg);

    emit finishStitch(stitchImg);
}

//--------------------------------------------
imageProcesser::~imageProcesser()
{

}
