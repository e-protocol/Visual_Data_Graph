#include <data_handler.h>

DataHandler::DataHandler(Graph *scene)
{
    m_scene = scene;
    dialog->setGeometry((m_scene->width() + 400) / 2, m_scene->height() / 2, 400,150);
}

DataHandler::~DataHandler()
{
    delete progressDialog;
    delete dialog;
    delete m_scene;
    delete file;
}

void DataHandler::loadData()
{
    //File Dialog
    QString path = QFileDialog::getOpenFileName(dialog,("Open File"),"/home");
    file = new QFile(path);

    //read data from file
    if (file->exists())
    {
        //check file suffix
        QFileInfo fileInfo(*file);
        QString suffix = fileInfo.suffix();
        if(suffix != "ssd" && suffix != "txt")
        {
            QMessageBox::critical(dialog,"Error","Invalid File format! \nSupported only: .ssd .txt\n");
            return;
        }

        //open file
        fileSize = file->size();
        file->open(QIODevice::ReadOnly);
        header.clear();
        m_scene->clearData();

        //get data from file
        if(readDataAlgorithm(file))
        {
            m_scene->clear();
            m_scene->drawGraphData(header);
        }
        else
            m_scene->clearData();

        //check errors
        checkErrors(path);

        //set progressDialog text
        progressDialog->setLabelText("Drawing Graph...");
        file->close();
    }
    else
    {
        //if path invalid
        if(!path.isEmpty())
            QMessageBox::critical(dialog,"Error","Invalid Path");
        //if cancel button pressed
        else
            return;
    }
    //close progressDialog
    progressDialog->setValue(fileSize);
}

bool DataHandler::readDataAlgorithm(QFile *file)
{
    //create progressDialog, will close in drawGraph() after drawing all data
    progressDialog = new QProgressDialog("Reading data...","Cancel",0,fileSize);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(10);

    int countBytes = 0;
    timestamp = 0.0;
    prev_timestamp = -0.1; //for first iteration
    QTextStream data(file);

    while (!data.atEnd())
    {
        QString input = QString::fromLatin1(file->readLine());
        is_timestamp = false;

        //get number from string
        if(!getNumber(input))
            return false;

        //progressDialog show progress
        countBytes += input.size(); // sizeof measurement data string
        if(!showProgress(countBytes))
            return false;
    }
    return true;
}

bool DataHandler::showProgress(int countBytes)
{
    if(countBytes <= 1) //to avoid negative progressBar
        countBytes = 1;
    progressDialog->setValue(countBytes - 1);
    if(progressDialog->wasCanceled())
    {
        header.clear();
        m_scene->clearData(); //clear data
        progressDialog->setValue(fileSize);
        return false;
    }
    return true;
}

void DataHandler::checkErrors(const QString &path)
{
    //check for Errors
    if(header.isEmpty())
    {
        progressDialog->setValue(fileSize);
        QMessageBox::critical(dialog,"Error","Invalid headers in file: " + path);
        return;
    }
    if(!m_scene->checkDataEmpty())
    {
        progressDialog->setValue(fileSize);
        QMessageBox::critical(dialog,"Error","Invalid data in file: " + path);
        return;
    }
}

bool DataHandler::getNumber(const QString &input)
{
    QString upload = "";
    int min = -1; //number of first symbol
    int max = 0; //number of last symbol = space
    for(int k = 0; k < input.size(); k++)
    {
        //save File header
        if(input[k] == '#')
        {
            QString str = input;
            header += str.remove(0,1);
            break;
        }

        if(min == -1 && input[k] != ' ')
        {
            min = k;
            continue;
        }
        if(min != -1 && max == 0 && (input[k] == ' ' || input[k] == '\n'))
            max = k;
        if(max != 0)
        {
            for(int j = min; j < max; j++)
                upload.push_back(input[j]);
            min = -1;
            max = 0;

            //skip empty string
            if(upload.size() < 3)
                continue;

            //check for convert error
            if(!convertStringToDouble(upload))
                return false;

            upload.clear();
            k--;
            //check on exit app
            if(is_Exit)
                return false; //exit app
        }
    }
    return true;
}

bool DataHandler::convertStringToDouble(const QString &upload)
{
    bool ok;
    double val = upload.toDouble(&ok); //convert string to number

    //check for convert error
    if(!ok && !checkSymbols(upload))
    {
        qDebug() << "Error: Couldn't convert string to double";
        return false;
    }

    if(!is_timestamp)
    {
        //time can't be negative or flow in opposite direction
        if(val < 0.0 || val < prev_timestamp)
            return false;

        is_timestamp = true;
        timestamp = val;
    }
    else
    {
        m_scene->setDataTimestamp(timestamp);
        m_scene->setDataValue(val);
        prev_timestamp = timestamp;
    }
    return true;
}

bool DataHandler::checkSymbols(const QString &upload)
{
    char symbols[] = {'0','1','2','3','4','5','6','7','8','9','.','-','+','e'};
    for(int k = 0; k < upload.size(); k++)
    {
        bool found = false;
        for(size_t j = 0; j < sizeof(symbols); j++)
        {
            if(upload[k].toLatin1() == symbols[j])
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }
    return true;
}
