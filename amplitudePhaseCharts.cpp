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
        for (qreal omega(0); omega < 100; omega+=0.001) {   //just calculate very big omega
            QPointF value = this->getValueInOmegaPoint(data, omega);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(omega);
            dataList << Data(value, label);
            xValVector.push_back(value.x());
            yValVector.push_back(value.y());
        }
        result << dataList;
    }

    this->setBorderValues(data, xValVector, yValVector);

    return result;
}

/*******************************************************************************\
 *                                   Private
\*******************************************************************************/

QPointF AmplitudePhaseCalculation::getValueInOmegaPoint(DataAcquired_t& data,
                                                      qreal anglePoint)
{
    QPointF result;

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
        result.setX(0);
        result.setY(0);
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
QPointF AmplitudePhaseCalculation::getProportional(DataAcquired_t& data,
                                                   qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion First Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionFirstOrder(DataAcquired_t& data,
                                                          qreal omega)
{
    QPointF result;
    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX( data.k / ( pow((qreal)data.t1, 2) * pow(omega, 2) + (qreal)1 ) );
        result.setY( (-data.k * (qreal)data.t1 * omega)/( pow((qreal)data.t1, 2) * pow(omega, 2) + 1 ) );
    }else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionSecondOrder(DataAcquired_t& data,
                                                           qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionThirdOrder(DataAcquired_t& data,
                                                          qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionFourthOrder(DataAcquired_t& data,
                                                           qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getDifferentiation(DataAcquired_t& data,
                                                      qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}

/*******************************************************************************\
 *                                Integration
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntegration(DataAcquired_t& data,
                                                  qreal omega)
{
    QPointF result;

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }
    else
    {
        result.setX(0); //FIXME temp
        result.setY(0); //FIXME temp
    }

    return result;
}
