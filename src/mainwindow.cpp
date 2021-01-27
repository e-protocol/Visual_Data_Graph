#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings(this); //initiate settings
    scene = new Graph(this,settings);
    view = new QGraphicsView(this);

    //create menuBar
    createMenu();

    //set window parameters
    int width = this->width();
    int height = this->height();
    view->setGeometry(0, centralWidget()->height() - menu_height, width, height);
    scene->setSceneRect(0, 0, view->width() * window_factor, view->height() * window_factor);
    dataHandler = new DataHandler(scene);
    view->setScene(scene);
    view->setRenderHints(QPainter::Antialiasing); //set smooth lines
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->showMaximized();
    setFixedSize(width,height); //disable Window resize

    //set information
    setInformation();
}

MainWindow::~MainWindow()
{
    delete ui;
    dataHandler->~DataHandler();
}

void MainWindow::about()
{
    QMessageBox::information(this,"About","This is an example of Visual Data representation. Created by E-Protocol");
}

void MainWindow::quit()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton question;
    question = QMessageBox::question(this,"Exit","Are you sure you want to exit?",QMessageBox::Yes | QMessageBox::No);
    if(question == QMessageBox::Yes)
    {
        dataHandler->setIsExit(true);
        qApp->exit(0);
    }
    else
        event->ignore();
}

void MainWindow::openFile()
{
    //load data from file
    this->setDisabled(true); //disable mainwindow
    dataHandler->loadData();
    this->setDisabled(false); //enable mainwindow
}

//change Settings
void MainWindow::changeSettings()
{
    int value = settings->getPrecision();
    if(settings->exec() == QDialog::Accepted)
        settings->setPrecision(settings->sliderPrecision->value());
    else
    {
        settings->sliderPrecision->setValue(value);
        settings->setPrecision(value);
        settings->textPrecision->setText("Precision: " + QString::number(value));
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    //QRectF rect = scene->itemsBoundingRect();
    //scene->setSceneRect(rect);

    view->setFixedWidth(event->size().width());
    view->setFixedHeight(event->size().height());

    //view->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    view->centerOn(0,0);

    scene->setSceneRect(0, 0, view->width() * window_factor, view->height() * window_factor);
}

void MainWindow::setInformation()
{
    //set information at start
    QGraphicsTextItem *textData = scene->addText("No Data to draw. To Draw graph go to: \nFile->Load Data or Press Alt + F + L");
    QFont f;
    f.setPointSize(14);
    textData->setFont(f);
    textData->setDefaultTextColor(Qt::black);
    textData->setTextWidth(textDataWidth);
    int sceneWidth = this->width();
    int sceneHeight = this->height();
    textData->setPos(sceneWidth,sceneHeight / 2);
}

void MainWindow::createMenu()
{
    menuBar = new QMenuBar;
    //menuBar File
    //Information
    QMenu * menu = new QMenu("&File");
    menu->addAction("&About",this, &MainWindow::about);
    //Load data
    menu->addAction("&Load Data",this, &MainWindow::openFile);
    //Change settings
    menu->addAction("&Settings",this,&MainWindow::changeSettings);
    //Exit
    menu->addAction("&Exit",this, &MainWindow::quit);
    menuBar->addMenu(menu);
    this->setMenuBar(menuBar);
}
