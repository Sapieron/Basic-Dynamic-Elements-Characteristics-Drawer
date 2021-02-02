#include "calculation.hpp"
#include <cmath>

using Calculation::Calculator;
using Calculation::MemberType_t;
using Calculation::ResponseType_t;
using Calculation::DataAcquired_t;

qreal Calculator::calculate(DataAcquired_t& data,
                            int timerStamp)
{
    qreal result;

    switch(data.memberType)
    {
    case MemberType_t::Proportional:
        result = this->getProportional(data, timerStamp);
        break;
    case MemberType_t::InertionFirstOrder:
        result = this->getIntertionFirstOrder(data, timerStamp);
        break;
    case MemberType_t::InertionSecondOrder:
        result = this->getIntertionSecondOrder(data, timerStamp);
        break;
    case MemberType_t::InertionThirdOrder:
        result = this->getIntertionThirdOrder(data, timerStamp);
        break;
    case MemberType_t::InertionFourthOrder:
        result = this->getIntertionFourthOrder(data, timerStamp);
        break;
    case MemberType_t::Differentiation:
        result = this->getDifferentiation(data, timerStamp);
        break;
    case MemberType_t::Integration:
        result = this->getIntegration(data, timerStamp);
        break;
    }

    return result;
}

/*******************************************************************************\
 *                                Proportional
\*******************************************************************************/
qreal Calculator::getProportional(DataAcquired_t& data,
                                  int timeStamp)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        // zostawiam 0 bo impuls Diraca = nieskonczonosc dla t=0
        // i 0 dla jakiegokolwiek innego t
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
qreal Calculator::getIntertionFirstOrder(DataAcquired_t& data,
                                         int timeStamp)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return data.k / qreal(data.t1) * exp(-(qreal)timeStamp/(qreal)data.t1);
    }else
    {
        return data.k * (1 - exp(-(qreal)timeStamp/(qreal)data.t1));
    }
}

/*******************************************************************************\
 *                            Inertion Second Order
\*******************************************************************************/
qreal Calculator::getIntertionSecondOrder(DataAcquired_t& data,
                                          int timeStamp)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return ((qreal)data.k / ((qreal)data.t1 - (qreal)data.t2)) * ( exp(-(qreal)timeStamp/(qreal)data.t1) - exp(-(qreal)timeStamp / (qreal)data.t2) ) ;
    }
    else
    {
        return (qreal)data.k*( (qreal)1 - ((qreal)data.t1/((qreal)data.t1 - (qreal)data.t2)) * exp(-(qreal)timeStamp / (qreal)data.t1) +  ((qreal)data.t2/((qreal)data.t1 - (qreal)data.t2)) * exp(-(qreal)timeStamp / (qreal)data.t2)) ;
    }
}

/*******************************************************************************\
 *                            Inertion Third Order
\*******************************************************************************/
qreal Calculator::getIntertionThirdOrder(DataAcquired_t& data,
                                          int timeStamp)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return data.k - (pow((qreal)data.t1, 2)*data.k*exp(-(qreal)timeStamp/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) + (pow((qreal)data.t2, 2)*data.k*exp(-(qreal)timeStamp/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) - (pow((qreal)data.t3, 2)*data.k*exp(-(qreal)timeStamp/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3));
    }
    else
    {
        return ((qreal)data.t1*(qreal)data.k*exp(-(qreal)timeStamp/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timeStamp/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timeStamp/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t1*data.k*exp(-timeStamp/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)) - ((qreal)data.t2*data.k*exp(-timeStamp/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)) + ((qreal)data.t3*data.k*exp(-timeStamp/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3));

    }
}

/*******************************************************************************\
 *                            Inertion Fourth Order
\*******************************************************************************/
qreal Calculator::getIntertionFourthOrder(DataAcquired_t& data,
                                          int timeStamp)
{
    if(data.responseType == ResponseType_t::Impulse)
    {
        return (pow((qreal)data.t1, 2)*data.k*exp(-timeStamp/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) - (pow((qreal)data.t2, 2)*data.k*exp(-timeStamp/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) + (pow((qreal)data.t3, 2)*data.k*exp(-timeStamp/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) - (pow((qreal)data.t4, 2)*data.k*exp(-timeStamp/(qreal)data.t4))/(((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4));
    }
    else
    {
        return data.k - (pow((qreal)data.t1, 3)*data.k*exp(-timeStamp/(qreal)data.t1))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t1 - (qreal)data.t4)) + (pow((qreal)data.t2, 3)*data.k*exp(-timeStamp/(qreal)data.t2))/(((qreal)data.t1 - (qreal)data.t2)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t4)) - (pow((qreal)data.t3, 3)*data.k*exp(-timeStamp/(qreal)data.t3))/(((qreal)data.t1 - (qreal)data.t3)*((qreal)data.t2 - (qreal)data.t3)*((qreal)data.t3 - (qreal)data.t4)) + (pow((qreal)data.t4, 3)*data.k*exp(-timeStamp/(qreal)data.t4))/(((qreal)data.t1 - (qreal)data.t4)*((qreal)data.t2 - (qreal)data.t4)*((qreal)data.t3 - (qreal)data.t4));
    }
}

/*******************************************************************************\
 *                              Differentiation
\*******************************************************************************/
qreal Calculator::getDifferentiation(DataAcquired_t& data,
                                     int timeStamp)
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
qreal Calculator::getIntegration(DataAcquired_t& data,
                                     int timeStamp)
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
