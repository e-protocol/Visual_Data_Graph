#ifndef FLOAT_NUM_CONV_H
#define FLOAT_NUM_CONV_H

#include <QString>
#include <QStringList>

class ConvertNumber
{
public:
    //convert data range to graph range
    double convertValueRange(double value_to_convert,
                             double range_input_min,
                             double range_input_max,
                             double range_output_min,
                             double range_output_max)
    {
         return (value_to_convert - range_input_min) /
                 (range_input_max - range_input_min) *
                 (range_output_max - range_output_min) +
                 range_output_min;
    }

    //round value to %5 == 0
    double roundValue(double value)
    {
        std::string str = std::to_string((int)value);;

        //if first digit is 0
        if(str[0] - '0' == 0)
            return value;

        //rewrite number
        QString output = "5";;
        if(str[0] - '0' != 5)
        {
            for(size_t k = 1; k < str.size(); k++)
                output += "0";
        }
        return output.toDouble();
    }

    //count number of digits in number
    int getCountOfDigits(double number)
    {
        std::string str = std::to_string(number);
        return str.size();
    }
};

#endif // FLOAT_NUM_CONV_H
