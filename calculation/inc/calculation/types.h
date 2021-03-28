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

    enum IdealRealType_t
    {
        Ideal = 0x00U,
        Real  = 0x01U
    };

    enum CharacteristicType_t
    {
        Time           = 0x01U,
        AmplitudePhase = 0x02U,
        PID            = 0x03U
    };

    enum FeedbackType_t
    {
        Positive = 0x00U,
        None     = 0x01U
    };

    struct DataAcquired_t
    {
        qreal k  = 0;
        qreal t1 = 0;
        qreal t2 = 0;
        qreal t3 = 0;
        qreal t4 = 0;

        qreal td = 0;
        qreal ti = 0;

        qreal kp         = 0;
        qreal ki         = 0;
        qreal kd         = 0;
        qreal dt         = 0;
        qreal target     = 0;
        qreal startPoint = 0;

        int maxT = 0;

        MemberType_t         memberType;
        ResponseType_t       responseType;
        IdealRealType_t      idealRealType;
        CharacteristicType_t characteristicType;
        FeedbackType_t       feedbackType;
    };
}

#endif // TYPES_H
