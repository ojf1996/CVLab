#include <QImage>
#include "imageProcesser.h"
#include "foo.h"
#include <QDebug>

//---------------------------------------
imageProcesser::imageProcesser(QObject *parent):
        QObject(parent)
{
}

//--------------------------------------
void imageProcesser::HomomorphicFiltering(std::string file)
{
    cv::Mat _src = cv::imread(file,CV_LOAD_IMAGE_UNCHANGED);

    cv::Mat out;
    QImage _out;


    cv::cvtColor(_src,_src,CV_BGR2RGB);
    Foo::cvMatToQImage(_src,_out);

    emit finishHomomorphicFiletering(_out);
}
//--------------------------------------------------------------------------------
void imageProcesser::ORBTest(std::string input1,std::string input2)
{
    cv::Mat in1,in2,out;
    QImage matchImg;

    in1 = cv::imread(input1,cv::IMREAD_UNCHANGED);
    in2 = cv::imread(input2,cv::IMREAD_UNCHANGED);

    //cv::Ptr<cv::ORB> orb= cv::ORB::create();
    cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create();
    std::vector<cv::KeyPoint> key1, key2;
    cv::Mat desc1,desc2;
    //orb->detectAndCompute(in1,cv::Mat(),key1,desc1);
    //orb->detectAndCompute(in2,cv::Mat(),key2,desc2);
    surf->detectAndCompute(in1,cv::Mat(),key1,desc1);
    surf->detectAndCompute(in2,cv::Mat(),key2,desc2);

    std::vector<cv::DMatch> matches;
    Foo::_2nnMatch(desc1,desc2,matches);

    /*
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create("BruteForce");
    matcher->match(desc1,desc2,matches);
    */
    Foo::ransac(in1,in2,out,matches,key1,key2);

    cv::drawMatches(in1,key1,in2,key2,matches,out);

    Foo::cvMatToQImage(out,matchImg);

    emit finishMatch(matchImg);
}

//--------------------------------------------
imageProcesser::~imageProcesser()
{

}
