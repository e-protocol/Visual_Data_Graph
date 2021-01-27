#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QProgressDialog>
#include <QString>
#include <QDebug>
#include <graph.h>

class DataHandler
{
private:
    QString header;
    QProgressDialog *progressDialog;
    QDialog *dialog = new QDialog;
    QFile * file;
    int fileSize;
    bool is_Exit = false;
    Graph *m_scene;
    double timestamp, prev_timestamp;
    bool is_timestamp;

    bool showProgress(int countBytes);
    void checkErrors(const QString &path);
    bool convertStringToDouble(const QString &upload);
    bool getNumber(const QString &input);
    bool checkSymbols(const QString &upload);
    bool readDataAlgorithm(QFile *file);

public:
    DataHandler(Graph *scene);
    ~DataHandler();
    void loadData();

    //getters and setters
    void setIsExit(bool par) {is_Exit = par;}
    QString getHeader() {return header;}
};

#endif // DATA_HANDLER_H
