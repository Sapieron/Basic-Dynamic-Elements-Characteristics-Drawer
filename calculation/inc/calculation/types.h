#ifndef TYPES_H
#define TYPES_H

#include "qpoint.h"
#include "qstring.h"
#include <QPair>

namespace Calculation
{
    using Data = QPair<QPointF, QString>;
    using DataList = QList<Data>;
    using DataTable = QList<DataList>;

    enum MemberType_t
    {
        Proportional        = 0x01U,
        InertionFirstOrder  = 0x02U,
        InertionSecondOrder = 0x03U,
        InertionThirdOrder  = 0x04U,
        InertionFourthOrder = 0x05U,
        Integration         = 0x06U,
        Differentiation     = 0x07U
    };

    enum ResponseType_t
    {
        Step    = 0x01U,
        Impulse = 0x02U
    };

    enum CharacteristicType_t
    {
        Time = 0x01U,
        AmplitudePhase = 0x02U
    };

    struct DataAcquired_t
    {
        qreal k  = 0;
        qreal t1 = 0;
        qreal t2 = 0;
        qreal t3 = 0;
        qreal t4 = 0;
        qreal td = 0;

        MemberType_t         memberType;
        ResponseType_t       responseType;
        CharacteristicType_t characteristicType;
    };
}

#endif // TYPES_H
