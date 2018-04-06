#include <QFile>
#include <QApplication>

#ifndef QSSLOADER_H
#define QSSLOADER_H

class QSSLoader
{
public:
    //@remark 这个函数用于读取QSS文件
    //@param style是对应QSS文件的名称
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};

#endif // QSSLOADER_H
