/**
 * Copyright 2019 Bradley J. Snyder <snyder.bradleyj@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @note modified by pawelochojec@gmail.com
 */

#ifndef _PID_SOURCE_
#define _PID_SOURCE_

#include <iostream>
#include <cmath>
#include "pidCalc.hpp"


#include <QPair>
#include <QPointF>
#include <QList>

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;


using Calculation::PIDCalculation;


DataTable PIDCalculation::calculate(DataAcquired_t& data,
                                    QPair<int, int> timeSpan)
{
    DataTable result;
    DataList dataList;

    _pv        = data.startPoint;
    _integral  = 0;
    _prevError = 0;
    _setpoint  = data.target;

    //TODO temporary, hardcoded _min and _max
    _min = -10;
    _max = 10;

    qreal step = 0.1;  //FIXME step is hardcoded, maybe user should define it? As sampling frequency? -> Ask Professor

    {
        dataList << Data(QPointF(0, data.startPoint), "Slice 0:0");

        for (qreal t(timeSpan.first + step); t < timeSpan.second; t += step)
        {
            QPointF value = this->getValueInTimePoint(data, t);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(t);
            dataList << Data(value, label);
        }
        result << dataList;
    }

    return result;
}


QPointF PIDCalculation::getValueInTimePoint( DataAcquired_t& data,
                                             qreal timePoint)
{
    QPointF result;

    switch(data.feedbackType)
    {
    case FeedbackType_t::None:
        result = getPIDWithNoFeedbackValue(data, timePoint);
        break;

    case FeedbackType_t::Positive:
        result = getPIDWithFeedbackValue(data, timePoint);
        break;
    }

    return result;
}


QPointF PIDCalculation::getPIDWithFeedbackValue(DataAcquired_t& data,
                                                qreal timePoint )
{
    QPointF result;

    qreal localResult;

    // Calculate error
    qreal error = _setpoint - _pv;

    // Proportional term
    qreal Pout = data.kp * error;

    // Integral term
    _integral += error * data.dt;
    qreal Iout = data.ki * _integral;

    // Derivative term
    qreal derivative = (error - _prevError) / data.dt;
    qreal Dout = data.kd * derivative;

    // Calculate total output
    localResult = Pout + Iout + Dout;

    // Restrict to max/min
    if( localResult > _max )
        localResult = _max;
    else if( localResult < _min )
        localResult = _min;

    // Save error to previous error
    _prevError = error;

    _pv += localResult;

    result.setX(timePoint);
    result.setY(localResult);
    return result;
}

QPointF PIDCalculation::getPIDWithNoFeedbackValue(DataAcquired_t& data,
                                                qreal timePoint)
{
    QPointF result;
//    Impulse:
//    kp/Ti + (kp*dirac(t)*(T + Td))/T - (Td*kp*exp(-t/T))/T^2

    if(data.responseType == ResponseType_t::Impulse)
    {
        result.setX(timePoint);
        result.setY( data.kp/data.ti +
                     (data.kp * diracDelta(timePoint) * (data.t1 + data.td))/data.t1 -
                     (data.td * data.kp * exp(-timePoint/data.t1))/ pow(data.t1, (qreal)2U) );
    }
    else
    {
        result.setX(timePoint);
        result.setY( data.kp + (data.kp * timePoint)/data.ti + (data.td * data.kp * exp(-timePoint/data.t1))/data.t1 );
    }

    return result;
}


#endif
