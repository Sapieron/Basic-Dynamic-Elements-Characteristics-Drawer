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
                yValue = this->getValueinTimePoint(data, t);
                QPointF value( t, yValue);
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

qreal TimeChartCalculation::getValueinTimePoint(DataAcquired_t& data,
                                                qreal timePoint)
{
    qreal result;

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
        result = 0;
        break;
    }

    return result;
}

/*******************************************************************************\
 *                                Proportional
\*******************************************************************************/
qreal TimeChartCalculation::getProportional(DataAcquired_t& data,
                                            qreal timePoint)
{
    Q_UNUSED(timePoint);

    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //FIXME shouldn't show anything
    }
    else
    {
        return (qreal)data.k;
    }
}

/*******************************************************************************\
 *                            Inertion First Order
\*******************************************************************************/
qreal TimeChartCalculation::getIntertionFirstOrder(DataAcquired_t& data,
                                                   qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return data.k / qreal(data.t1) * exp(-(qreal)timePoint/(qreal)data.t1);
    }else
    {
        return data.k * (1 - exp(-(qreal)timePoint/(qreal)data.t1));
    }
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
qreal TimeChartCalculation::getIntertionSecondOrder(DataAcquired_t& data,
                                                    qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return ((qreal)data.k / ((qreal)data.t1 - (qreal)data.t2)) * ( exp(-(qreal)timePoint/(qreal)data.t1) - exp(-(qreal)timePoint / (qreal)data.t2) ) ;
    }
    else
    {
        return (qreal)data.k*( (qreal)1 - ((qreal)data.t1/((qreal)data.t1 - (qreal)data.t2)) * exp(-(qreal)timePoint / (qreal)data.t1) +  ((qreal)data.t2/((qreal)data.t1 - (qreal)data.t2)) * exp(-(qreal)timePoint / (qreal)data.t2)) ;
    }
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
qreal TimeChartCalculation::getIntertionThirdOrder(DataAcquired_t& data,
                                                   qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return ((qreal)data.t1*(qreal)data.k*exp(-(qreal)timePoint/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timePoint/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timePoint/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t1*data.k*exp(-timePoint/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timePoint/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timePoint/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3));
    }
    else
    {
        return data.k - (pow((qreal)data.t1, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) + (pow((qreal)data.t2, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) - (pow((qreal)data.t3, 2)*data.k*exp(-(qreal)timePoint/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3));
    }
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
qreal TimeChartCalculation::getIntertionFourthOrder(DataAcquired_t& data,
                                                    qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return (pow((qreal)data.t1, 2)*data.k*exp(-timePoint/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) - (pow((qreal)data.t2, 2)*data.k*exp(-timePoint/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) + (pow((qreal)data.t3, 2)*data.k*exp(-timePoint/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) - (pow((qreal)data.t4, 2)*data.k*exp(-timePoint/(qreal)data.t4))/(((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4));
    }
    else
    {
        return data.k - (pow((qreal)data.t1, 3)*data.k*exp(-timePoint/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) + (pow((qreal)data.t2, 3)*data.k*exp(-timePoint/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) - (pow((qreal)data.t3, 3)*data.k*exp(-timePoint/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) + (pow((qreal)data.t4, 3)*data.k*exp(-timePoint/(qreal)data.t4))/(((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4));
    }
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
qreal TimeChartCalculation::getDifferentiation(DataAcquired_t& data,
                                               qreal timePoint)
{
    Q_UNUSED(timePoint);

    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //FIXME shouldn't show anything
    }
    else
    {
        return 0; //FIXME shouldn't show anything
    }
}

/*******************************************************************************\
 *                                Integration
\*******************************************************************************/
qreal TimeChartCalculation::getIntegration(DataAcquired_t& data,
                                           qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return (qreal)data.k;
    }
    else
    {
        return (qreal)data.k * (qreal)timePoint;
    }
}
