#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMouseEvent>
#include <settings.h>
#include <data_handler.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void about();
    void quit();
    void openFile();
    void changeSettings();

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    Graph *scene;
    QMenuBar *menuBar;
    QString header;
    Settings *settings;
    DataHandler *dataHandler;
    int fileSize;
    const int textDataWidth = 400; //width of header
    const double window_factor = 0.98; //to set graphicview borders
    const int menu_height = 7; //menuBar height

    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void setInformation();
    void createMenu();
};
#endif // MAINWINDOW_H
