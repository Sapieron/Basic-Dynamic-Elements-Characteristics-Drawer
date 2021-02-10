#include "timeCharts.hpp"
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
        std::vector<qreal> xValVector;
        std::vector<qreal> yValVector;

        {
            qreal yValue(0);
            for (qreal t(timeSpan.first); t < timeSpan.second; t+=(qreal)(0.01)) {
                QPointF value = this->getValueinTimePoint(data, t);
                QString label = "Slice " + QString::number(0) + ":" + QString::number(t);
                dataList << Data(value, label);
                xValVector.push_back(t);
                yValVector.push_back(yValue);
            }
            result << dataList;
        }

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
        result.setY( data.k / qreal(data.t1) * exp(-(qreal)timePoint/(qreal)data.t1) );
    }else
    {
        result.setX(timePoint);
        result.setY( data.k * (1 - exp(-(qreal)timePoint/(qreal)data.t1)) );
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
        result.setY( ((qreal)data.k / ((qreal)data.t1 - (qreal)data.t2)) *
                     ( exp(-(qreal)timePoint/(qreal)data.t1) - exp(-(qreal)timePoint / (qreal)data.t2) ) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( (qreal)data.k*( (qreal)1 - ((qreal)data.t1/((qreal)data.t1 - (qreal)data.t2)) *
                                     exp(-(qreal)timePoint / (qreal)data.t1) +  ((qreal)data.t2/((qreal)data.t1 - (qreal)data.t2)) *
                                     exp(-(qreal)timePoint / (qreal)data.t2)) );
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
        result.setY( ((qreal)data.t1*(qreal)data.k*exp(-(qreal)timePoint/(qreal)data.t1))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timePoint/(qreal)data.t2))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timePoint/(qreal)data.t3))/
                     (((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t1*data.k*exp(-timePoint/(qreal)data.t1))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timePoint/(qreal)data.t2))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timePoint/(qreal)data.t3))/
                     (((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k - (pow((qreal)data.t1, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t1))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) +
                     (pow((qreal)data.t2, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t2))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) -
                     (pow((qreal)data.t3, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t3))/
                     (((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)) );
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
        result.setY( (pow((qreal)data.t1, 2)*data.k*exp(-timePoint/(qreal)data.t1))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) -
                     (pow((qreal)data.t2, 2)*data.k*exp(-timePoint/(qreal)data.t2))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) +
                     (pow((qreal)data.t3, 2)*data.k*exp(-timePoint/(qreal)data.t3))/
                     (((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) -
                     (pow((qreal)data.t4, 2)*data.k*exp(-timePoint/(qreal)data.t4))/
                     (((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4)) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.k - (pow((qreal)data.t1, 3)*data.k*exp(-timePoint/(qreal)data.t1))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) +
                     (pow((qreal)data.t2, 3)*data.k*exp(-timePoint/(qreal)data.t2))/
                     (((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) -
                     (pow((qreal)data.t3, 3)*data.k*exp(-timePoint/(qreal)data.t3))/
                     (((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) +
                     (pow((qreal)data.t4, 3)*data.k*exp(-timePoint/(qreal)data.t4))/
                     (((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4)) );
    }

    return result;
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
QPointF TimeChartCalculation::getDifferentiation(DataAcquired_t& data,
                                               qreal timePoint)
{
    Q_UNUSED(data);
    Q_UNUSED(timePoint);

    QPointF result(0, 0);

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
        result.setY( (qreal)data.k );
    }
    else
    {
        result.setX(timePoint);
        result.setY( (qreal)data.k * (qreal)timePoint );
    }

    return result;
}
