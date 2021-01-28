#include "calculations.hpp"
#include <cmath>

using Calculation::Calculatator;
using Calculation::DataTable;
using Calculation::DataList;
using Calculation::Data;

Calculatator::Calculatator()
{
    //Nothing to do here
}

DataTable* Calculatator::GetInertionFirstOrder(Calculation::DataAcquired_t data,
                                               Calculation::ResponseType_t response)
{
    //this is the only one that works properly
    DataTable* result;
    DataList*  dataList;
    {
        qreal yValue(0);
        for (int t(0); t < 10; t++) {   //FIXME 10 is fixed now
            yValue = data.k * ((1 - exp(-qreal(t)/qreal(data.t1))))*qreal(1);
            QPointF value((qreal) t, yValue);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(t);
            dataList << Data(value, label);
        }
        result << dataList;
    }
    return result;
}
