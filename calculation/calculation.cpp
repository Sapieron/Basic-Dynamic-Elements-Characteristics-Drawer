#include <QList>
#include "types.h"
#include "calculation.hpp"
#include <cmath>

using Calculation::Calculator;
using Calculation::CharacteristicType_t;
using Calculation::DataAcquired_t;
using Calculation::DataTable;

/*******************************************************************************\
 *                                  Public
\*******************************************************************************/
DataTable Calculator::calculate(DataAcquired_t& data, QPair<int, int> span)
{
    DataTable result;

    switch (data.characteristicType)
    {
    case CharacteristicType_t::Time:
        result = this->_timeChartCalculation.calculate(data, span);
        break;

    case CharacteristicType_t::AmplitudePhase:
        {
        DataTable calculatedData = this->_amplitudePhaseChartCalculation.calculate(data, span);
        result = calculatedData;
        result << calculateXAxis(&calculatedData);
        result << calculateYAxis(&calculatedData);
        }
        break;

    case CharacteristicType_t::PID:
        {
        DataTable calculatedData = this->_pidCalculation.calculate(data, span);
        result = calculatedData;
        result << calculateXAxis(&calculatedData);
        result << calculateYAxis(&calculatedData);
        }
        break;

    default:
        break;
    }

    return result;
}

DataList Calculator::calculateXAxis(DataTable *dataTable)
{
    DataList dataList = dataTable->at(0);
    DataList list;

    auto minX = std::min_element(dataList.begin(),
                                 dataList.end(),
                                 [](const Data &p1,
                                 const Data &p2){
        return p1.first.x() < p2.first.x();
    });

    auto maxX = std::max_element(dataList.begin(),
                                 dataList.end(),
                                 [](const Data &p1,
                                 const Data &p2){
        return p1.first.x() < p2.first.x();
    });

    QPointF minXValue(minX->first.x(), 0);
    QPointF maxXValue(maxX->first.x(), 0);

    list << Data(minXValue, "Min") << Data(maxXValue, "Max");

    return list;
}

DataList Calculator::calculateYAxis(DataTable *dataTable)
{
    DataList dataList = dataTable->at(0);
    DataList list;

    auto minY = std::min_element(dataList.begin(),
                                 dataList.end(),
                                 [](const Data &p1,
                                 const Data &p2){
        return p1.first.y() < p2.first.y();
    });

    auto maxY = std::max_element(dataList.begin(),
                                 dataList.end(),
                                 [](const Data &p1,
                                 const Data &p2){
        return p1.first.y() < p2.first.y();
    });

    QPointF minYValue(0, minY->first.y());
    QPointF maxYValue(0, maxY->first.y());

    list << Data(minYValue, "Min") << Data(maxYValue, "Max");

    return list;
}
