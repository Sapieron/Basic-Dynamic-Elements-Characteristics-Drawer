#include "timeCalc.hpp"
#include <cmath>
#include <qlist.h>

using Calculation::TimeChartCalculation;
using Calculation::DataAcquired_t;
using Calculation::DataTable;

/*******************************************************************************\
 *                                   Public
\*******************************************************************************/
DataTable TimeChartCalculation::calculate(DataAcquired_t& data, QPair<int, int> timeSpan)
{
        DataTable result;
        DataList dataList;

        qreal samplingFrequency = 0.1;

        for (qreal t(timeSpan.first); t < timeSpan.second; t += samplingFrequency) {
            QPointF value = this->getValueinTimePoint(data, t);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(t);
            dataList << Data(value, label);
        }
        result << dataList;


        return result;
}

/*******************************************************************************\
 *                                   Private
\*******************************************************************************/

QPointF TimeChartCalculation::getValueinTimePoint(DataAcquired_t& data,
                                                qreal timePoint)
{
    QPointF result;

    switch(data.memberType)
    {
    case MemberType_t::Proportional:
        result = this->getProportional(data, timePoint);
        break;
    case MemberType_t::InertionFirstOrder:
        result = this->getIntertionFirstOrder(data, timePoint);
        break;
    case MemberType_t::InertionSecondOrder:
        result = this->getIntertionSecondOrder(data, timePoint);
        break;
    case MemberType_t::InertionThirdOrder:
        result = this->getIntertionThirdOrder(data, timePoint);
        break;
    case MemberType_t::InertionFourthOrder:
        result = this->getIntertionFourthOrder(data, timePoint);
        break;
    case MemberType_t::Differentiation:
        result = this->getDifferentiation(data, timePoint);
        break;
    case MemberType_t::Integration:
        result = this->getIntegration(data, timePoint);
        break;

    default:
        break;
    }

    return result;
}

/*******************************************************************************\
 *                                Proportional
\*******************************************************************************/
QPointF TimeChartCalculation::getProportional(DataAcquired_t& data,
                                            qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0);
        result.setY(0);
    }
    else
    {
        result.setX(timePoint);
        result.setY(data.k);
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion First Order
\*******************************************************************************/
QPointF TimeChartCalculation::getIntertionFirstOrder(DataAcquired_t& data,
                                                     qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( data.k / data.t1 * exp(-timePoint/data.t1) );
    }else
    {
        result.setX(timePoint);
        result.setY( data.k * ( (qreal)1 - exp(-timePoint/data.t1)) );
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
QPointF TimeChartCalculation::getIntertionSecondOrder(DataAcquired_t& data,
                                                      qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( (data.k / (data.t1 - data.t2)) *
                     ( exp(-timePoint/data.t1) - exp(-timePoint / data.t2) ) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k*( (qreal)1 - (data.t1/(data.t1 - data.t2)) *
                              exp(-timePoint / data.t1) +  (data.t2/(data.t1 - data.t2)) *
                              exp(-timePoint / data.t2)) );
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
QPointF TimeChartCalculation::getIntertionThirdOrder(DataAcquired_t& data,
                                                     qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( (data.t1*data.k*exp(-timePoint/data.t1))/
                     ((data.t1 - data.t2)*(data.t1 - data.t3)) - (data.t2*data.k*exp(-timePoint/data.t2))/
                     ((data.t1 - data.t2)*(data.t2 - data.t3)) + (data.t3*data.k*exp(-timePoint/data.t3))/
                     ((data.t1 - data.t3)*(data.t2 - data.t3)) + (data.t1*data.k*exp(-timePoint/data.t1))/
                     ((data.t1 - data.t2)*(data.t1 - data.t3)) - (data.t2*data.k*exp(-timePoint/data.t2))/
                     ((data.t1 - data.t2)*(data.t2 - data.t3)) + (data.t3*data.k*exp(-timePoint/data.t3))/
                     ((data.t1 - data.t3)*(data.t2 - data.t3)) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k - (pow(data.t1, (qreal)2)*data.k*exp(-timePoint/data.t1))/
                     ((data.t1 - data.t2)*(data.t1 - data.t3)) +
                     (pow(data.t2, (qreal)2)*data.k*exp(-timePoint/data.t2))/
                     ((data.t1 - data.t2)*(data.t2 - data.t3)) -
                     (pow(data.t3, (qreal)2)*data.k*exp(-timePoint/data.t3))/
                     ((data.t1 - data.t3)*(data.t2 - data.t3)) );
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
QPointF TimeChartCalculation::getIntertionFourthOrder(DataAcquired_t& data,
                                                      qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( (pow(data.t1, (qreal)2)*data.k*exp(-timePoint/data.t1))/
                     ((data.t1 - data.t2)*(data.t1 - data.t3)*(data.t1 - data.t4)) -
                     (pow(data.t2, (qreal)2)*data.k*exp(-timePoint/data.t2))/
                     ((data.t1 - data.t2)*(data.t2 - data.t3)*(data.t2 - data.t4)) +
                     (pow(data.t3, (qreal)2)*data.k*exp(-timePoint/data.t3))/
                     ((data.t1 - data.t3)*(data.t2 - data.t3)*(data.t3 - data.t4)) -
                     (pow(data.t4, (qreal)2)*data.k*exp(-timePoint/data.t4))/
                     ((data.t1 - data.t4)*(data.t2 - data.t4)*(data.t3 - data.t4)) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k - (pow(data.t1, (qreal)3)*data.k*exp(-timePoint/data.t1))/
                     ((data.t1 - data.t2)*(data.t1 - data.t3)*(data.t1 - data.t4)) +
                     (pow(data.t2, (qreal)3)*data.k*exp(-timePoint/data.t2))/
                     ((data.t1 - data.t2)*(data.t2 - data.t3)*(data.t2 - data.t4)) -
                     (pow(data.t3, (qreal)3)*data.k*exp(-timePoint/data.t3))/
                     ((data.t1 - data.t3)*(data.t2 - data.t3)*(data.t3 - data.t4)) +
                     (pow(data.t4, (qreal)3)*data.k*exp(-timePoint/data.t4))/
                     ((data.t1 - data.t4)*(data.t2 - data.t4)*(data.t3 - data.t4)) );
    }

    return result;
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
QPointF TimeChartCalculation::getDifferentiation(DataAcquired_t& data,
                                                 qreal timePoint)
{
    QPointF result;

    if(data.idealRealType == IdealRealType_t::Ideal)
    {
        result.setX(0);
        result.setY(0);
    }
    else
    {
        if(data.responseType == ResponseType_t::Impulse)
        {
            result.setX( timePoint );
            result.setY( (data.k*exp(-timePoint/data.td))/data.td );
        }
        else
        {
            result.setX( timePoint );
            result.setY( (data.k * diracDelta(timePoint))/data.td -
                         (data.k * exp(-timePoint/data.td))/pow(data.td, (qreal)2U) );
        }
    }

    return result;
}

/*******************************************************************************\
 *                                Integration
\*******************************************************************************/
QPointF TimeChartCalculation::getIntegration(DataAcquired_t& data,
                                             qreal timePoint)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( data.k );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k * timePoint );
    }

    return result;
}
