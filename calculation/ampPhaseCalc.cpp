#include "ampPhaseCalc.hpp"
#include <cmath>
#include <qlist.h>
#include <QtGui/QPen>

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

    qreal samplingFrequency = 0.001;

    {
        //just calculate very big omega
        for (qreal omega(samplingFrequency); omega < 100; omega += samplingFrequency)
        {
            QPointF value = this->getValueInOmegaPoint(data, omega);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(omega);
            dataList << Data(value, label);
        }

        result << dataList;
    }

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


/*******************************************************************************\
 *                                Proportional
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getProportional(DataAcquired_t& data,
                                                   qreal omega)
{
    //To avoid ,,unused" warnings
    Q_UNUSED(omega);

    QPointF result;

    result.setX(data.k);
    result.setY(0);

    return result;
}

/*******************************************************************************\
 *                            Inertion First Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionFirstOrder(DataAcquired_t& data,
                                                          qreal omega)
{
    QPointF result;

    result.setX( data.k / ( pow(data.t1, (qreal)2) * pow(omega, (qreal)2) + (qreal)1 ) );
    result.setY( (-data.k * data.t1 * omega)/( pow(data.t1, (qreal)2) * pow(omega, (qreal)2) + (qreal)1 ) );

    return result;
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionSecondOrder(DataAcquired_t& data,
                                                           qreal omega)
{
    QPointF result;

    result.setX( data.k * ( (qreal)1 - pow(omega, (qreal)2) * data.t1 * data.t2 ) /
                 ( (qreal)1 + pow(( omega * data.t1 ), (qreal)2) +
                   pow(( omega * data.t2 ), (qreal)2) +
                   pow( pow(omega, (qreal)2) * data.t1 * data.t2, (qreal)2) ) );
    result.setY( -data.k * omega * ( data.t1 + data.t2 ) /
                 ( (qreal)1 + pow(( omega * data.t1 ), (qreal)2)
                   + pow(( omega * data.t2 ), (qreal)2) +
                   pow( pow(omega, (qreal)2) * data.t1 * data.t2, (qreal)2) ) );


    return result;
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionThirdOrder(DataAcquired_t& data,
                                                          qreal omega)
{
    QPointF result;

    result.setX(
                data.k * ( ( (qreal)1 - pow(omega, (qreal)2) * data.t1 * data.t2 ) -
                           (pow(omega, (qreal)2) * data.t1 * data.t3) -
                           (pow(omega, (qreal)2) * data.t1 * data.t2) )
                /
                ( ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t1, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t2, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t3, (qreal)2) ) )
                );

    result.setY(
                data.k * ( (-omega * data.t3) -
                           (omega * data.t1) -
                           (omega * data.t2) +
                           ( pow(omega, (qreal)3) * data.t1 * data.t2 * data.t3) ) /
                ( ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t1, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t2, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t3, (qreal)2) ) )
                );

    return result;
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getIntertionFourthOrder(DataAcquired_t& data,
                                                           qreal omega)
{
    QPointF result;

    result.setX(
                data.k * ( ( (qreal)1 - pow(omega, (qreal)2) * data.t3 * data.t4 ) -
                           (pow(omega, (qreal)2) * data.t1 * data.t4) -
                           (pow(omega, (qreal)2) * data.t1 * data.t2) -
                           (pow(omega, (qreal)2) * data.t2 * data.t4) -
                           (pow(omega, (qreal)2) * data.t2 * data.t3) +
                           (pow(omega, (qreal)4) * data.t1 * data.t2 * data.t3 * data.t4 )
                           )
                /
                ( ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t1, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t2, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t3, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t4, (qreal)2) ) )
                );

    result.setY(
                data.k * ( (-omega * data.t4) -
                           (omega * data.t3) -
                           (omega * data.t1) +
                           ( pow(omega, (qreal)3) * data.t1 * data.t2 * data.t4) -
                           (omega * data.t2) +
                           (pow(omega, (qreal)3) * data.t2 * data.t3 * data.t4) +
                           (pow(omega, (qreal)3) * data.t1 * data.t2 * data.t4) +
                           (pow(omega, (qreal)3) * data.t1 * data.t2 * data.t3)
                           )
                /
                ( ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t1, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t2, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t3, (qreal)2) ) *
                  ( (qreal)1 + pow(omega, (qreal)2) * pow(data.t4, (qreal)2) ) )
                );

    return result;
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
QPointF AmplitudePhaseCalculation::getDifferentiation(DataAcquired_t& data,
                                                      qreal omega)
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
            //TODO impulse
        }
        else
        {
            //TODO step
        }
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

    result.setX(0);
    result.setY( (qreal)data.td * omega );

    return result;
}
