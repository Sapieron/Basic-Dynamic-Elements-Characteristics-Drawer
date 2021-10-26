#ifndef AMPLITUDEPHASECHARTS_HPP
#define AMPLITUDEPHASECHARTS_HPP

#include "calculations/ICalculations.hpp"

namespace Calculation
{
    class AmplitudePhaseCalculation : public ICalculation
    {
      public:
        AmplitudePhaseCalculation() = default;

        virtual DataTable calculate(DataAcquired_t& data, QPair<int, int> span) override;

      private:
        /**
         * @brief getPoint Get value in angle
         * @param data
         * @param omega
         * @return
         */
        QPointF getValueInOmegaPoint(DataAcquired_t& data,
                                     qreal omega);

        /**
         * @brief getProportional calculates data in angle related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getProportional(DataAcquired_t& data,
                                qreal omega);

        /**
         * @brief getIntertionFirstOrder calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getIntertionFirstOrder(DataAcquired_t& data,
                                       qreal omega);

        /**
         * @brief getIntertionSecondOrder calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getIntertionSecondOrder(DataAcquired_t& data,
                                        qreal omega);

        /**
         * @brief getIntertionThirdOrder calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getIntertionThirdOrder(DataAcquired_t& data,
                                       qreal omega);

        /**
         * @brief getIntertionFourthOrder calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getIntertionFourthOrder(DataAcquired_t& data,
                                        qreal omega);

        /**
         * @brief getDifferentiation calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getDifferentiation(DataAcquired_t& data,
                                   qreal omega);

        /**
         * @brief getIntegration calculates data in angle point related on data provided
         * @param data
         * @param omega
         * @return computed value
         */
        QPointF getIntegration(DataAcquired_t& data,
                               qreal omega);
    };
}


#endif // AMPLITUDEPHASECHARTS_HPP
