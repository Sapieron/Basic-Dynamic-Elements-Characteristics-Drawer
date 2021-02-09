#ifndef TIMECHARTS_HPP
#define TIMECHARTS_HPP

#include "ICalculator.hpp"  //FIXME actually that interfacing won't be needed, I'm not using it anywhere
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
         * @brief setBorderValues sets border values of x and y
         * @param data
         * @param xValVector
         * @param yValVector
         */
        void setBorderValues(Calculation::DataAcquired_t& data,
                             std::vector<qreal> xValVector,
                             std::vector<qreal> yValVector);

        /**
         * @brief getPoint Returns time point
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getValueinTimePoint(DataAcquired_t& data,
                                   qreal timeStamp);

        /**
         * @brief getProportional calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return computed value
         */
        qreal getProportional(DataAcquired_t& data,
                              qreal timeStamp);

        /**
         * @brief getIntertionFirstOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionFirstOrder(DataAcquired_t& data,
                                     qreal timeStamp);

        /**
         * @brief getIntertionSecondOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionSecondOrder(DataAcquired_t& data,
                                      qreal timeStamp);

        /**
         * @brief getIntertionThirdOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionThirdOrder(DataAcquired_t& data,
                                     qreal timeStamp);

        /**
         * @brief getIntertionFourthOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionFourthOrder(DataAcquired_t& data,
                                      qreal timeStamp);

        /**
         * @brief getDifferentiation calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getDifferentiation(DataAcquired_t& data,
                                 qreal timeStamp);

        /**
         * @brief getIntegration calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntegration(DataAcquired_t& data,
                             qreal timeStamp);
    };
}

#endif // TIMECHARTS_HPP
