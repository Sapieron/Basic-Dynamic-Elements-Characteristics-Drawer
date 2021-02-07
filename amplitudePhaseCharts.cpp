#include "amplitudePhaseCharts.hpp"
#include <cmath>
#include <qlist.h>

using Calculation::AmplitudePhaseCalculation;
using Calculation::DataAcquired_t;
using Calculation::DataTable;

/*******************************************************************************\
 *                                   Public
\*******************************************************************************/
DataTable AmplitudePhaseCalculation::calculate(DataAcquired_t& data, QPair<int, int> timeSpan)
{
    Q_UNUSED(timeSpan);

    DataTable result;
    DataList dataList;
    std::vector<qreal> xValVector;
    std::vector<qreal> yValVector;

    {
        qreal yValue(0);
        for (qreal angle(0); angle < 360; angle+=(qreal)(0.01)) {   //FIXME temporary, will be calculated differently
            yValue = this->getValueInAnglePoint(data, angle);
            QPointF value( angle, yValue);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(angle);
            dataList << Data(value, label);
            xValVector.push_back(angle);
            yValVector.push_back(yValue);
        }
        result << dataList;
    }

    this->setBorderValues(data, xValVector, yValVector);

    return result;
}

/*******************************************************************************\
 *                                   Private
\*******************************************************************************/

qreal AmplitudePhaseCalculation::getValueInAnglePoint(DataAcquired_t& data,
                                                     qreal anglePoint)
{
    qreal result;

    switch(data.memberType)
    {
    case MemberType_t::Proportional:
        result = this->getProportional(data, anglePoint);
        break;
    case MemberType_t::InertionFirstOrder:
        result = this->getIntertionFirstOrder(data, anglePoint);
        break;
    case MemberType_t::InertionSecondOrder:
        result = this->getIntertionSecondOrder(data, anglePoint);
        break;
    case MemberType_t::InertionThirdOrder:
        result = this->getIntertionThirdOrder(data, anglePoint);
        break;
    case MemberType_t::InertionFourthOrder:
        result = this->getIntertionFourthOrder(data, anglePoint);
        break;
    case MemberType_t::Differentiation:
        result = this->getDifferentiation(data, anglePoint);
        break;
    case MemberType_t::Integration:
        result = this->getIntegration(data, anglePoint);
        break;

    default:
        result = 0;
        break;
    }

    return result;
}


void AmplitudePhaseCalculation::setBorderValues(Calculation::DataAcquired_t& data,
                                                std::vector<qreal> xValVector,
                                                std::vector<qreal> yValVector)
{

    data.minXValue = *std::min_element(xValVector.begin(), xValVector.end());
    data.maxXValue = *std::max_element(xValVector.begin(), xValVector.end());
    data.minYValue = *std::min_element(yValVector.begin(), yValVector.end());
    data.maxYValue = *std::max_element(yValVector.begin(), yValVector.end());

    return;
}

/*******************************************************************************\
 *                                Proportional
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getProportional(DataAcquired_t& data,
                                                 qreal anglePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //FIXME temp
    }
    else
    {
        return 0; //FIXME temp
    }

}

/*******************************************************************************\
 *                            Inertion First Order
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getIntertionFirstOrder(DataAcquired_t& data,
                                                        qreal anglePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return data.k / qreal(data.t1) * exp(-(qreal)anglePoint/(qreal)data.t1); //TODO temporary, it's here just to see if anything gets calculated
    }else
    {
        return 0; //TODO add this
    }
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getIntertionSecondOrder(DataAcquired_t& data,
                                                         qreal anglePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //TODO add this
    }
    else
    {
        return 0; //TODO add this
    }
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getIntertionThirdOrder(DataAcquired_t& data,
                                                        qreal anglePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0;   //TODO add this
    }
    else
    {
        return 0;  //TODO add this

    }
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getIntertionFourthOrder(DataAcquired_t& data,
                                                         qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //TODO add this
    }
    else
    {
        return 0; //TODO add this
    }
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getDifferentiation(DataAcquired_t& data,
                                                    qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //FIXME temp
    }
    else
    {
        return 0; //FIXME temp
    }
}

/*******************************************************************************\
 *                                Integration
\*******************************************************************************/
qreal AmplitudePhaseCalculation::getIntegration(DataAcquired_t& data,
                                                qreal timePoint)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return 0; //FIXME temp
    }
    else
    {
        return 0; //FIXME temp
    }
}
