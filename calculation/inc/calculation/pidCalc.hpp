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

#ifndef _PID_H_
#define _PID_H_

#include <QPair>
#include <QPointF>

#include "ICalculator.hpp"

namespace Calculation
{
    class PIDCalculation : public ICalculator
    {
    public:
        PIDCalculation() = default;
        ~PIDCalculation() = default;

        virtual DataTable calculate(DataAcquired_t& data,
                                    QPair<int, int> span) override;

    private:
        QPointF getValueInTimePoint(DataAcquired_t& data,
                                    qreal timePoint );

        QPointF getPIDWithFeedbackValue(DataAcquired_t& data,
                                        qreal timePoint );

        QPointF getPIDWithNoFeedbackValue(DataAcquired_t& data,
                                          qreal timePoint);

        /**
         * @brief _pv - process value, contains current value of PID
         */
        qreal _pv;

        qreal _integral;

        qreal _prevError;

        qreal _min;

        qreal _max;

        qreal _setpoint;
    };
}

#endif
