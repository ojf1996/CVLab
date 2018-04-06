#include "View.h"
#include "qssloader.h"
#include <QApplication>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSSLoader::setStyle(":/res/resource/qss/stylesheet.qss");
    View w;
    w.show();


    return a.exec();
}
