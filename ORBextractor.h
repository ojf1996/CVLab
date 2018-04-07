#pragma once

#include <vector>
#include <list>
#include <opencv/cv.h>


//定义提取节点
class ExtractorNode {
public:
	ExtractorNode() :bNoMore(false) {}

	void DivideNode(ExtractorNode &n1, ExtractorNode &n2, ExtractorNode &n3, ExtractorNode &n4);

	std::vector<cv::KeyPoint> vKeys;
	cv::Point2i UL, UR, BL, BR;
	std::list<ExtractorNode>::iterator lit;
	bool bNoMore;
};

class ORBextractor {
public:
	ORBextractor(int nfeatures = 500, float scaleFactor = 1.2f, int nlevels = 8,
		int iniThFAST = 20, int minThFAST = 7);

	~ORBextractor() {}

	// 计算ORB特征点和描述子，实现中没有使用到mask
	void operator()(cv::InputArray image, cv::InputArray mask,
		std::vector<cv::KeyPoint>& keypoints,
		cv::OutputArray descriptors);


	//图像金字塔
	std::vector<cv::Mat> mvImagePyramid;

protected:
	//计算图像金字塔
	void ComputePyramid(cv::Mat image);
	//计算特征点四叉树
	void ComputeKeyPointsOctTree(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

	//使用四叉树分配特征点
	std::vector<cv::KeyPoint> DistributeOctTree(const std::vector<cv::KeyPoint>& vToDistributeKeys, const int &minX,
		const int &maxX, const int &minY, const int &maxY, const int &nFeatures, const int &level);

	std::vector<cv::Point> pattern;

	//需要提取的最大特征点数
	int nfeatures;
	//图像金字塔的尺度参数
	double scaleFactor;
	//高斯图像金字塔的层数
	int nlevels;
	//默认fast角点阈值
	int iniThFAST;
	//最小fast角点阈值
	int minThFAST;

	//每层图像的特征点数
	std::vector<int> mnFeaturesPerLevel;


	std::vector<int> umax;

	//存储每层图像的尺度因子
	std::vector<float> mvScaleFactor;
};
