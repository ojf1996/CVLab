#include "View.h"
#include <qDebug>
#include <QPaintEvent>
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
    processer(new imageProcesser(this)),
    _width(600),
    _height(400),
    photo1(QImage()),
    currPhoto(&photo1)
{
    ui->setupUi(this);
    //重置大小
    this->resize(620,500);

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
    ORBMenu = new QMenu(tr("ORB特征提取与匹配"),this);
    OpMenu->addMenu(ORBMenu);
    SIFTMenu = new QMenu(tr("SIFT特征提取与匹配"),this);
    OpMenu->addMenu(SIFTMenu);
    SURFMenu = new QMenu(tr("SURF特征提取与匹配"),this);
    OpMenu->addMenu(SURFMenu);

    //添加操作
    ORBWith2NN = new QAction(tr("进行野点剔除"),this);
    ORBWithout2NN = new QAction(tr("不进行野点剔除"),this);
    ORBMenu->addAction(ORBWith2NN);
    ORBMenu->addAction(ORBWithout2NN);

    SIFTWith2NN = new QAction(tr("进行野点剔除"),this);
    SIFTWithout2NN = new QAction(tr("不进行野点剔除"),this);
    SIFTMenu->addAction(SIFTWith2NN);
    SIFTMenu->addAction(SIFTWithout2NN);

    SURFWith2NN = new QAction(tr("进行野点剔除"),this);
    SURFWithout2NN = new QAction(tr("不进行野点剔除"),this);
    SURFMenu->addAction(SURFWith2NN);
    SURFMenu->addAction(SURFWithout2NN);

    //中央布局
    QWidget* temp = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setAlignment(Qt::AlignLeft);
    vbox->setAlignment(Qt::AlignCenter);

    //添加图片展示区域
    Photo = new QLabel(this);
    Photo->resize(QSize(int(600/1.2),int(500/1.2)));
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
    connect(ORBWith2NN,SIGNAL(triggered(bool)),this,SLOT(startORBWith2NN()));
    connect(ORBWithout2NN,SIGNAL(triggered(bool)),this,SLOT(startORBWithout2NN()));
    connect(SIFTWith2NN,SIGNAL(triggered(bool)),this,SLOT(startSIFTWith2NN()));
    connect(SIFTWithout2NN,SIGNAL(triggered(bool)),this,SLOT(startSIFTWithout2NN()));
    connect(SURFWith2NN,SIGNAL(triggered(bool)),this,SLOT(startSURFWith2NN()));
    connect(SURFWithout2NN,SIGNAL(triggered(bool)),this,SLOT(startSURFWithout2NN()));
    //进行传递
    connect(this,SIGNAL(startMatch(std::string,std::string,int,bool)),processer,SLOT(matchTest(std::string,std::string,int,bool)));
    connect(processer,SIGNAL(finishMatch(const QImage&)),this,SLOT(loadResultPhoto(const QImage&)));
    connect(processer,SIGNAL(finishDetectPhoto1(const QImage&)),this,SLOT(updatePhoto1(const QImage&)));
    connect(processer,SIGNAL(finishDetectPhoto2(const QImage&)),this,SLOT(updatePhoto2(const QImage&)));
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
        currPhoto = &photo1;
        Photo->setPixmap(QPixmap::fromImage(photo1).scaled(QSize(_width,_height),Qt::KeepAspectRatio));
    }
    else if(which == 2)
    {
        currPhoto = &photo2;
        Photo->setPixmap(QPixmap::fromImage(photo2).scaled(QSize(_width,_height),Qt::KeepAspectRatio));
    }
    else if(which == 3)
    {
        if(!resPhoto.isNull()){
            currPhoto = &resPhoto;
            Photo->setPixmap(QPixmap::fromImage(resPhoto).scaled(QSize(_width,_height),Qt::KeepAspectRatio));
        }
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

void View::paintEvent(QPaintEvent* event)
{
    qDebug()<<_width<<"  "<<_height<<"\n";
    _width =int(size().width() / 1.2);
    _height =int(size().height() / 1.2);
    Photo->resize(QSize(_width,_height));
    if(!currPhoto->isNull())
        Photo->setPixmap(QPixmap::fromImage(*currPhoto).scaled(QSize(_width,_height),Qt::KeepAspectRatio));
    Photo->move(int(size().width() * (1 - 1/ 1.2f) * 0.5),30);
    photo1Btn->move(Photo->pos().x(),_height + Photo->pos().y());
    photo2Btn->move(photo1Btn->pos().x()+photo1Btn->size().width(),_height + Photo->pos().y());
    resultBtn->move(photo2Btn->pos().x()+photo2Btn->size().width(),_height + Photo->pos().y());
    QMainWindow::paintEvent(event);
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
void View::updatePhoto1(const QImage & res)
{
    if(res.isNull()){
        QMessageBox::critical(this,"发生了错误","图片处理失败");
        return;
    }

    photo1 = res.copy();
    emit showPhoto1();
}
//-----------------------------------------------------
void View::updatePhoto2(const QImage & res)
{
    if(res.isNull()){
        QMessageBox::critical(this,"发生了错误","图片处理失败");
        return;
    }

    photo2 = res.copy();
    emit showPhoto2();
}
//-----------------------------------------------------
void View::showResultPhoto()
{
    emit showPhoto(3);
}
//-----------------------------------------------------
void View::startORBWith2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),1,true);
}
//----------------------------------------------------
void View::startORBWithout2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),1,false);
}
//-----------------------------------------------------
void View::startSIFTWith2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),2,true);
}
//----------------------------------------------------
void View::startSIFTWithout2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),2,false);
}
//-----------------------------------------------------
void View::startSURFWith2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),3,true);
}
//----------------------------------------------------
void View::startSURFWithout2NN()
{
    if(file1.isNull() || file2.isNull())
    {
        QMessageBox::critical(this,"发生了错误","请确保两张输入图片");
        return;
    }
    emit startMatch(file1.toStdString(),file2.toStdString(),3,false);
}
