#ifndef ICALCULATOR_HPP
#define ICALCULATOR_HPP

#include "QPair"    //FIXME use std::lib!
#include "QPointF"

#include "types.h"

//FIXME if we really need this, than it should be defined in types.hpp or something!
using Data = QPair<QPointF, QString>;   //FIXME prefer stdlib over qt libs
using DataList = QList<Data>;
using DataTable = QList<DataList>;

namespace Calculation
{
    class ICalculation  //FIXME it's not an interface, it's not ppurely abstract!
    {
      public:
        virtual ~ICalculation() = delete;

        virtual DataTable calculate(DataAcquired_t& data, QPair<int, int> span) = 0;

        qreal diracDelta(qreal timePoint) const;
    };
}

#endif // ICALCULATOR_HPP
