#ifndef TYPES_H
#define TYPES_H

#include "qpoint.h"
#include "qstring.h"
#include "qpair.h"

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

    struct DataAcquired_t
    {
        uint32_t k  = 0;
        uint32_t t1 = 0;
        uint32_t t2 = 0;
        uint32_t t3 = 0;
        uint32_t t4 = 0;
    };
}

#endif // TYPES_H
