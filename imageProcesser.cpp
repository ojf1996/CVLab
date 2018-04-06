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
void imageProcesser::ORBTest(std::string input1,std::string input2)
{
    cv::Mat in1,in2,out,_in1,_in2;
    QImage matchImg,in1Img,in2Img;
    //由于QImage转cv：：Mat后，虽然可以显示，可是存在一些问题，故直接读取算了
    in1 = cv::imread(input1,cv::IMREAD_UNCHANGED);
    in2 = cv::imread(input2,cv::IMREAD_UNCHANGED);

    cv::Ptr<cv::ORB> orb= cv::ORB::create();

    std::vector<cv::KeyPoint> key1, key2;
    cv::Mat desc1,desc2;
    orb->detectAndCompute(in1,cv::Mat(),key1,desc1);
    orb->detectAndCompute(in2,cv::Mat(),key2,desc2);

    Foo::myDrawKeypoint(in1,key1,_in1);
    Foo::cvMatToQImage(_in1,in1Img);
    emit finishDetectPhoto1(in1Img);

    Foo::myDrawKeypoint(in2,key2,_in2);
    Foo::cvMatToQImage(_in2,in2Img);
    emit finishDetectPhoto2(in2Img);

    std::vector<cv::DMatch> matches;
    Foo::_2nnMatch(desc1,desc2,matches);

    Foo::ransac(in1,in2,out,matches,key1,key2);

    cv::drawMatches(in1,key1,in2,key2,matches,out);

    Foo::cvMatToQImage(out,matchImg);

    emit finishMatch(matchImg);
}

//--------------------------------------------
imageProcesser::~imageProcesser()
{

}
