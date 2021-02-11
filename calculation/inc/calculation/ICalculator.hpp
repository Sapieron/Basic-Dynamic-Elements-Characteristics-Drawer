#ifndef ICALCULATOR_HPP
#define ICALCULATOR_HPP
#include "QPair"
#include "QPointF"

#include "types.h"

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;

namespace Calculation
{
    class ICalculator
    {
    public:
        virtual DataTable calculate(DataAcquired_t& data, QPair<int, int> span) = 0;
    };
}

#endif // ICALCULATOR_HPP
