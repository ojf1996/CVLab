#include <QMainWindow>
#include "imageProcesser.h"

class QLabel;
class QPixmap;
class QPushButton;

#pragma execution_character_set("utf-8")
namespace Ui {
class View;
}

#ifndef VIEW_H
#define VIEW_H

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();

private:
    Ui::View *ui;

protected:
    //两张图片的文件路径
    QString file1;
    QString file2;
    //两张图片内容
    QImage photo1;
    QImage photo2;
    QImage resPhoto;

protected:
    //打开文件菜单的各种操作
    QAction *File1OpenMenu;
    QAction *File2OpenMenu;
    QAction *File1CloseMenu;
    QAction *File2CloseMenu;

    //图像处理菜单
    QAction *HomomorphicFilteringMenu;

protected:
    //菜单栏
    //文件菜单
    QMenu *FileMenu;
    QMenu *OpMenu;
    //图片
    QLabel* Photo;
    QPushButton *photo1Btn;
    QPushButton *photo2Btn;
    QPushButton *resultBtn;

    imageProcesser *processer;
signals:
    //@remark 更新图片的信号
    //@param which表明展示某张图片，1表示第一张，如此类推
    void showPhoto(int which);
    void ORBTest(std::string, std::string);

protected slots:
    //@remark 读取图片
    void loadPhoto1();
    void loadPhoto2();
    void loadResultPhoto(const QImage&);

    //@remark 更新PhotoLabel中的图片
    //@param which表明是那一副pixmap
    void updatePhotoLabel(int which);

    //@remark 点击下方按钮展示图片
    void showPhoto1();
    void showPhoto2();
    void showResultPhoto();

    //@remark 更新得出关键点和描述出之后的图片
    void updatePhoto1(const QImage&);

    //@remark 关闭标签
    void closePhoto1();
    void closePhoto2();

    //@remark test
    void test();
};

#endif // VIEW_H
