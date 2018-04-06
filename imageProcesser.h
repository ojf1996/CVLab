#include <QObject>
#include <string>
#include <vector>
#include "opencv2/imgproc.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

class QImage;

#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

class imageProcesser:public QObject
{
    Q_OBJECT
public:
    explicit imageProcesser(QObject* parent = nullptr);
    ~imageProcesser();
public slots:
    void HomomorphicFiltering(std::string file);
    void ORBTest(std::string,std::string);
signals:
    void finishHomomorphicFiletering(const QImage& res);
    void finishORBDetectInFirstImg(const QImage& out);
    void finishORDDetectInSecondImg(const QImage& out);
    void finishMatch(const QImage& out);
};

#endif // IMAGEPROCESSER_H
