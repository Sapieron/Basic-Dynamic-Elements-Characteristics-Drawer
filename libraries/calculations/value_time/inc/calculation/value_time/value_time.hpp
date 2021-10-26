#ifndef TIMECHARTS_HPP
#define TIMECHARTS_HPP

#include "calculation/ICalculation.hpp"
#include <vector>

namespace Calculation
{
    class TimeChartCalculation : public ICalculator
    {
      public:
        TimeChartCalculation() = default;

        virtual DataTable calculate(DataAcquired_t& data, QPair<int, int> span) override;

      private:
        /**
         * @brief getPoint Returns time point
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getValueinTimePoint(DataAcquired_t& data,
                                    qreal timeStamp);

        /**
         * @brief getProportional calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return computed value
         */
        QPointF getProportional(DataAcquired_t& data,
                                qreal timeStamp);

        /**
         * @brief getIntertionFirstOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getIntertionFirstOrder(DataAcquired_t& data,
                                       qreal timeStamp);

        /**
         * @brief getIntertionSecondOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getIntertionSecondOrder(DataAcquired_t& data,
                                        qreal timeStamp);

        /**
         * @brief getIntertionThirdOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getIntertionThirdOrder(DataAcquired_t& data,
                                       qreal timeStamp);

        /**
         * @brief getIntertionFourthOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getIntertionFourthOrder(DataAcquired_t& data,
                                        qreal timeStamp);

        /**
         * @brief getDifferentiation calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getDifferentiation(DataAcquired_t& data,
                                   qreal timeStamp);

        /**
         * @brief getIntegration calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        QPointF getIntegration(DataAcquired_t& data,
                               qreal timeStamp);
    };
}

#endif // TIMECHARTS_HPP
