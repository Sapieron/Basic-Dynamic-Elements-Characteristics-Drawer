#ifndef CALCULATION_HPP
#define CALCULATION_HPP

#include "types.h"

namespace Calculation
{
    class Calculator
    {
      public:
        /**
         * @brief calculate handles calculation of data based on member type set in data reference
         * @param data
         * @param timeStamp
         * @return
         */
        qreal calculate(DataAcquired_t& data,
                        int timeStamp);

      private:
        /**
         * @brief getProportional calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return computed value
         */
        qreal getProportional(DataAcquired_t& data,
                              int timeStamp);

        /**
         * @brief getIntertionFirstOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionFirstOrder(DataAcquired_t& data,
                                     int timeStamp);

        /**
         * @brief getIntertionSecondOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionSecondOrder(DataAcquired_t& data,
                                      int timeStamp);

        /**
         * @brief getIntertionThirdOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionThirdOrder(DataAcquired_t& data,
                                     int timeStamp);

        /**
         * @brief getIntertionFourthOrder calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntertionFourthOrder(DataAcquired_t& data,
                                      int timeStamp);

        /**
         * @brief getDifferentiation calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getDifferentiation(DataAcquired_t& data,
                                 int timeStamp);

        /**
         * @brief getIntegration calculates data at time point related on data provided
         * @param data
         * @param timeStamp
         * @return
         */
        qreal getIntegration(DataAcquired_t& data,
                             int timeStamp);
    };
}

#endif // CALCULATION_HPP
