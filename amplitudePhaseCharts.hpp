#ifndef AMPLITUDEPHASECHARTS_HPP
#define AMPLITUDEPHASECHARTS_HPP

#include "ICalculator.hpp"
#include "vector"

namespace Calculation
{
    class AmplitudePhaseCalculation : public ICalculator
    {
      public:
        AmplitudePhaseCalculation() = default;

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
         * @brief getPoint Get value in angle
         * @param data
         * @param anglePoint
         * @return
         */
        qreal getValueInAnglePoint(DataAcquired_t& data,
                                   qreal anglePoint);

        /**
         * @brief getProportional calculates data in angle related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getProportional(DataAcquired_t& data,
                              qreal anglePoint);

        /**
         * @brief getIntertionFirstOrder calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getIntertionFirstOrder(DataAcquired_t& data,
                                     qreal anglePoint);

        /**
         * @brief getIntertionSecondOrder calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getIntertionSecondOrder(DataAcquired_t& data,
                                      qreal anglePoint);

        /**
         * @brief getIntertionThirdOrder calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getIntertionThirdOrder(DataAcquired_t& data,
                                     qreal timeStamp);

        /**
         * @brief getIntertionFourthOrder calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getIntertionFourthOrder(DataAcquired_t& data,
                                      qreal anglePoint);

        /**
         * @brief getDifferentiation calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getDifferentiation(DataAcquired_t& data,
                                 qreal anglePoint);

        /**
         * @brief getIntegration calculates data in angle point related on data provided
         * @param data
         * @param anglePoint
         * @return computed value
         */
        qreal getIntegration(DataAcquired_t& data,
                             qreal anglePoint);
    };
}


#endif // AMPLITUDEPHASECHARTS_HPP
