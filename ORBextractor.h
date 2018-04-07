#pragma once

#include <vector>
#include <list>
#include <opencv/cv.h>


//������ȡ�ڵ�
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

	// ����ORB������������ӣ�ʵ����û��ʹ�õ�mask
	void operator()(cv::InputArray image, cv::InputArray mask,
		std::vector<cv::KeyPoint>& keypoints,
		cv::OutputArray descriptors);


	//ͼ�������
	std::vector<cv::Mat> mvImagePyramid;

protected:
	//����ͼ�������
	void ComputePyramid(cv::Mat image);
	//�����������Ĳ���
	void ComputeKeyPointsOctTree(std::vector<std::vector<cv::KeyPoint> >& allKeypoints);

	//ʹ���Ĳ�������������
	std::vector<cv::KeyPoint> DistributeOctTree(const std::vector<cv::KeyPoint>& vToDistributeKeys, const int &minX,
		const int &maxX, const int &minY, const int &maxY, const int &nFeatures, const int &level);

	std::vector<cv::Point> pattern;

	//��Ҫ��ȡ�������������
	int nfeatures;
	//ͼ��������ĳ߶Ȳ���
	double scaleFactor;
	//��˹ͼ��������Ĳ���
	int nlevels;
	//Ĭ��fast�ǵ���ֵ
	int iniThFAST;
	//��Сfast�ǵ���ֵ
	int minThFAST;

	//ÿ��ͼ�����������
	std::vector<int> mnFeaturesPerLevel;


	std::vector<int> umax;

	//�洢ÿ��ͼ��ĳ߶�����
	std::vector<float> mvScaleFactor;
};
