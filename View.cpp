#include "View.h"
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include "ui_view.h"
#include "foo.h"

//----------------------------------------------------
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View),
    processer(new imageProcesser(this))
{
    ui->setupUi(this);
    //重置大小
    this->resize(800,600);

    //添加打开文件的菜单
    FileMenu = menuBar()->addMenu("&文件");
    File1OpenMenu = new QAction(tr("&打开第一个文件"),this);
    FileMenu->addAction(File1OpenMenu);
    File2OpenMenu = new QAction(tr("&打开第二个文件"),this);
    FileMenu->addAction(File2OpenMenu);
    File1CloseMenu = new QAction(tr("&关闭第一个文件"),this);
    FileMenu->addAction(File1CloseMenu);
    File2CloseMenu = new QAction(tr("&关闭第二个文件"),this);
    FileMenu->addAction(File2CloseMenu);
    File1CloseMenu->setVisible(false);
    File2CloseMenu->setVisible(false);

    //添加操作的菜单
    OpMenu = menuBar()->addMenu("&操作");
    HomomorphicFilteringMenu = new QAction(tr("测试"),this);
    OpMenu->addAction(HomomorphicFilteringMenu);

    //中央布局
    QWidget* temp = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setAlignment(Qt::AlignLeft);
    vbox->setAlignment(Qt::AlignCenter);

    //添加图片展示区域
    Photo = new QLabel(this);
    Photo->setFixedSize(QSize(600,400));
    Photo->setObjectName("PhotoLabel");
    Photo->setAlignment(Qt::AlignCenter);

    //选择图片按钮
    photo1Btn = new QPushButton("图片1",this);
    photo2Btn = new QPushButton("图片2",this);
    resultBtn = new QPushButton("结果",this);
    photo1Btn->setObjectName("PhotoBtn");
    photo2Btn->setObjectName("PhotoBtn");
    resultBtn->setObjectName("PhotoBtn");
    photo2Btn->setMaximumSize(QSize(50,30));
    photo1Btn->setMaximumSize(QSize(50,30));
    resultBtn->setMaximumSize(QSize(50,30));
    photo1Btn->setVisible(false);
    photo2Btn->setVisible(false);
    resultBtn->setVisible(false);
    //添加元素
    hbox->addWidget(photo1Btn);
    hbox->addWidget(photo2Btn);
    hbox->addWidget(resultBtn);
    vbox->addWidget(Photo);
    vbox->addLayout(hbox);
    temp->setLayout(vbox);
    setCentralWidget(temp);

    //连接信号与槽
    //文件菜单信号
    connect(File1OpenMenu,SIGNAL(triggered(bool)),this,SLOT(loadPhoto1()));
    connect(File2OpenMenu,SIGNAL(triggered(bool)),this,SLOT(loadPhoto2()));
    connect(File1CloseMenu,SIGNAL(triggered(bool)),this,SLOT(closePhoto1()));
    connect(File2CloseMenu,SIGNAL(triggered(bool)),this,SLOT(closePhoto2()));
    //图片展示信号
    connect(this,SIGNAL(showPhoto(int)),this,SLOT(updatePhotoLabel(int)));
    connect(photo1Btn,SIGNAL(clicked(bool)),this,SLOT(showPhoto1()));
    connect(photo2Btn,SIGNAL(clicked(bool)),this,SLOT(showPhoto2()));
    connect(resultBtn,SIGNAL(clicked(bool)),this,SLOT(showResultPhoto()));

    //图片处理信号
    connect(HomomorphicFilteringMenu,SIGNAL(triggered(bool)),this,SLOT(test()));
    connect(this,SIGNAL(ORBTest(std::string,std::string)),processer,SLOT(ORBTest(std::string,std::string)));
    connect(processer,SIGNAL(finishMatch(const QImage&)),this,SLOT(loadResultPhoto(const QImage&)));
}
//--------------------------------------------------
void View::loadPhoto1()
{
    QString temp = QFileDialog::getOpenFileName(this,tr("Open File"),
                                         "/home",
                                         tr("Images (*.png *.jpg *.bmp *.tif *.jpeg)"));
    if(temp.isNull()){
        QMessageBox::critical(this,"警告","没有这张图片");
        return;
    }

    if(photo1.load(temp))
    {
        file1 = temp;
        photo1Btn->setVisible(true);
        File1CloseMenu->setVisible(true);
        emit showPhoto(1);
    }
    else
    {
        QMessageBox::critical(this,"发生了错误","图片读取失败");
    }
}
//--------------------------------------------------
void View::loadPhoto2()
{
    QString temp = QFileDialog::getOpenFileName(this,tr("Open File"),
                                         "/home",
                                         tr("Images (*.png *.jpg *.bmp *.tif *.jpeg)"));
    if(temp.isNull()){
        QMessageBox::critical(this,"警告","没有这张图片");
        return;
    }

    if(photo2.load(temp))
    {       
        file2 = temp;
        photo2Btn->setVisible(true);
        File2CloseMenu->setVisible(true);
        emit showPhoto(2);
    }
    else
    {
        QMessageBox::critical(this,"发生了错误","图片读取失败");
    }
}
//---------------------------------------------------
void View::updatePhotoLabel(int which)
{
    if(which == 1)
    {
        Photo->setPixmap(QPixmap::fromImage(photo1).scaled(QSize(600,600),Qt::KeepAspectRatio));
    }
    else if(which == 2)
    {
        Photo->setPixmap(QPixmap::fromImage(photo2).scaled(QSize(600,600),Qt::KeepAspectRatio));
    }
    else if(which == 3)
    {
        if(!resPhoto.isNull())
            Photo->setPixmap(QPixmap::fromImage(resPhoto).scaled(QSize(600,600),Qt::KeepAspectRatio));
    }
    else
    {
        return;
    }
}
//------------------------------------------------------
void View::showPhoto1()
{
    emit showPhoto(1);
}
//------------------------------------------------------
void View::showPhoto2()
{
    emit showPhoto(2);
}
//------------------------------------------------------
void View::closePhoto1()
{
    file1.clear();
    photo1Btn->setVisible(false);
    File1CloseMenu->setVisible(false);
    photo1 = QImage();
    emit showPhoto(2);
}
//-----------------------------------------------------
void View::closePhoto2()
{
    file2.clear();
    photo2Btn->setVisible(false);
    File2CloseMenu->setVisible(false);
    photo2 = QImage();
    emit showPhoto(1);
}
//------------------------------------------------------
View::~View()
{
    delete ui;
}
//------------------------------------------------------
void View::loadResultPhoto(const QImage& res)
{
    if(res.isNull()){
        QMessageBox::critical(this,"发生了错误","图片处理失败");
        resPhoto = QImage();
        resultBtn->setVisible(false);
        return;
    }

    resPhoto = res.copy();
    resultBtn->setVisible(true);
    emit showResultPhoto();
}
//-----------------------------------------------------
void View::showResultPhoto()
{
    emit showPhoto(3);
}
//-----------------------------------------------------
void View::test()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit ORBTest(file1.toStdString(),file2.toStdString());
}
