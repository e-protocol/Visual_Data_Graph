#include <graph.h>
#include <QMessageBox>
#include <QString>
#include <math.h>

Graph::Graph(QObject *parent, Settings *settings) :
    QGraphicsScene(parent)
{
    m_settings = settings;
}

void Graph::setGraphDimensions()
{
    graph_min_height = this->height() * graph_min_height_factor;
    graph_max_height = this->height() * graph_max_height_factor;

    int count = convertNumber->getCountOfDigits(max_element);
    graph_min_width = this->width() * graph_min_width_factor * (count + m_settings->getPrecision()); //count graph left indent according number of digits in value
    graph_max_width = this->width();
}

void Graph::drawGraphData(const QString &header)
{
    //find max and min elements
    findMaxMin();
    //set graph dimensions
    setGraphDimensions();
    //draw axis
    drawAxis(header);
    //draw intervals
    drawInterval_X();
    drawInterval_Y();

    for(int k = 1 ; k < data_value.size(); k++)
    {
        //timestamp coordinates
        //convert first dot X timestamp to graph range
        double first_x = data_timestamp.at(k - 1);
        first_x = convertNumber->convertValueRange(first_x,timestamp_min,timestamp_max,graph_min_width,graph_max_width);
        //convert second dot X timestamp to graph range
        double second_x = data_timestamp.at(k);
        second_x = convertNumber->convertValueRange(second_x,timestamp_min,timestamp_max,graph_min_width,graph_max_width);

        //value coordinates
        //convert first dot Y value to graph range
        double first_y = data_value.at(k - 1);
        first_y = convertNumber->convertValueRange(first_y, graph_center - distance, graph_center + distance, graph_max_height, graph_min_height);
        //convert second dot Y value to graph range
        double second_y = data_value.at(k);
        second_y = convertNumber->convertValueRange(second_y, graph_center - distance, graph_center + distance, graph_max_height, graph_min_height);

        //draw line
        //scene->width()*0.02 - starting coordinate X to draw
        this->addLine(first_x, first_y, second_x,second_y, QPen(Qt::red,2,Qt::SolidLine,Qt::SquareCap));
    }
}

void Graph::drawAxis(const QString &header)
{
    //view header
    QGraphicsTextItem *textData = this->addText(header);
    QFont f;
    f.setPointSize(10);
    textData->setFont(f);
    textData->setDefaultTextColor(Qt::black);
    textData->setTextWidth(header_width);
    textData->setPos(graph_min_width,0);

    //draw axis X - Timestamp
    //graph_min_width - starting coordinate X to draw
    this->addLine(graph_min_width,graph_max_height,graph_max_width,graph_max_height,QPen(Qt::black,1));

    //draw axis Y - Value
    //graph_min_width - starting coordinate X to draw
    this->addLine(graph_min_width,graph_max_height,graph_min_width,0,QPen(Qt::black,1));
}

void Graph::findMaxMin()
{
    timestamp_max = data_timestamp.at(data_timestamp.size() - 1);
    max_element = data_value.at(0);
    min_element = data_value.at(0);
    for(int k = 1; k < data_value.size(); k++)
    {
        if(data_value.at(k) > max_element)
            max_element = data_value.at(k);
        if(data_value.at(k) < min_element)
            min_element = data_value.at(k);
    }
    timestamp_max = timestamp_max * 1.1; //place free space after graph 10%
}

void Graph::drawInterval_X()
{
    //**************** X axis ****************
    //count X intervals value
    distance = timestamp_max;
    graph_center = timestamp_max - distance / 2;
    double coord_x;
    double coord_y = graph_max_height;

    double interval_length = (graph_max_width - graph_min_width) / interval_X_count; //5 lines below graph_center and 5 lines above graph_center
    double interval_value = distance / interval_X_count;
    double round_factor = round(interval_value * interval_X_count) / interval_X_count / interval_value; //count round_factor to round timestamp
    interval_value *= round_factor;

    //draw axis X intervals
    for(int k = 0; k < interval_X_count + 1; k++) // +1 to add timestamp to free space after graph
    {
        //add iterval line to X axis
        coord_x = graph_min_width + interval_length * k * round_factor; //find coordinate x value with rounding factor
        this->addLine(coord_x,coord_y,coord_x,coord_y + interval_line_length,QPen(Qt::black,1));

        //draw axis X value
        QGraphicsTextItem *textData = this->addText(QString::number(k * interval_value));
        QFont f;
        f.setPointSize(10);
        textData->setFont(f);
        textData->setDefaultTextColor(Qt::black);
        textData->setPos(coord_x - textData->boundingRect().width() / 2,coord_y + interval_line_length); //place text under interval line
    }
}

void Graph::drawInterval_Y()
{
    //**************** Y axis ****************
    //count Y intervals value
    distance = max_element - min_element;
    graph_center = max_element - distance / 2;
    double coord_x = graph_min_width;

    double interval_length = (graph_max_height - graph_min_height) / interval_Y_count; //3 lines below graph_center and 3 lines above graph_center
    double max_value = graph_center + distance;
    double interval_value = distance / 3;

    //show round values for estetic view
    if(m_settings->is_Round)
    {
        interval_value = convertNumber->roundValue(interval_value); // round number to %5
        max_value = ceil(graph_center / 1000) * 1000 + interval_value * 3;
        distance = interval_value * 3; //rounding values affect correct graph displaying
    }

    //draw axis Y intervals
    for(int k = 1; k < interval_Y_count + 1; k++) // +1 to add timestamp to free space after graph
    {
        //add iterval line to Y axis
        double coord_y = graph_min_height + interval_length * k; //find coordinate y value

        //check if interval line not equal to axis X to avoid double line draw
        if(k == interval_Y_count)
            this->addLine(coord_x,coord_y,coord_x - interval_line_length,coord_y,QPen(Qt::black,1));
        else
            this->addLine(coord_x - interval_line_length,coord_y,this->width(),coord_y,QPen(Qt::black,1));

        //draw axis Y value
        double y_value = max_value - k * interval_value;
        QGraphicsTextItem *textData = this->addText(QString::number(y_value,'f',m_settings->getPrecision()));
        QFont f;
        f.setPointSize(10);
        textData->setFont(f);
        textData->setDefaultTextColor(Qt::black);
        textData->setPos(coord_x - textData->boundingRect().width() - interval_line_length,coord_y - textData->boundingRect().height() / 2); //place text left to interval line
    }
}

void Graph::clearData()
{
    data_timestamp.clear();
    data_value.clear();
}

bool Graph::checkDataEmpty()
{
    if(data_value.isEmpty())
        return false;
    return true;
}
