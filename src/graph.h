#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <float_num_conv.h>
#include <settings.h>

class Graph : public QGraphicsScene
{
    Q_OBJECT
private:
    QVector <double> data_timestamp;
    QVector <double> data_value;

public:
    explicit Graph(QObject *parent = nullptr,Settings *settings = nullptr);
    void drawGraphData(const QString &header);
    void clearData();
    bool checkDataEmpty();

    //setters and getters
    void setDataTimestamp(double timestamp) {data_timestamp.push_back(timestamp);}
    void setDataValue(double value) {data_value.push_back(value);}

private:
    double max_element = 0.0, min_element = 0.0;
    double timestamp_min = 0.0, timestamp_max = 0.0;
    double distance = 0.0;
    double graph_center = 0.0;
    double graph_min_height = 0.0, graph_max_height = 0.0;
    double graph_min_width = 0.0, graph_max_width = 0.0;
    const double graph_min_height_factor = 0.08;
    const double graph_max_height_factor = 0.92;
    const double graph_min_width_factor = 0.003;
    const int header_width = 400;
    const int interval_X_count = 10;
    const int interval_Y_count = 6;
    const int interval_line_length = 5;
    Settings *m_settings;
    ConvertNumber *convertNumber;

    void setGraphDimensions();
    void findMaxMin();
    void drawInterval_X();
    void drawInterval_Y();
    void drawAxis(const QString &header);
};

#endif // GRAPH_H
