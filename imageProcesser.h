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
    void matchTest(std::string,std::string,int,bool);
signals:
    void finishMatch(const QImage& out);
    void finishDetectPhoto1(const QImage& out);
    void finishDetectPhoto2(const QImage& out);
};

#endif // IMAGEPROCESSER_H
