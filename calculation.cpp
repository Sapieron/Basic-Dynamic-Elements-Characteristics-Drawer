#include <QList>
#include "types.h"
#include "calculation.hpp"
#include <cmath>

using Calculation::Calculator;
using Calculation::CharacteristicType_t;
using Calculation::MemberType_t;
using Calculation::ResponseType_t;
using Calculation::DataAcquired_t;
using Calculation::DataTable;

/*******************************************************************************\
 *                                  Public
\*******************************************************************************/
DataTable Calculator::calculate(DataAcquired_t& data, QPair<int, int> span)
{
    DataTable result;

    switch (data.characteristicType)
    {
    case CharacteristicType_t::Time:
        result = this->_timeChartCalculation.calculate(data, span);
        break;

    case CharacteristicType_t::AmplitudePhase:
//        result = this->_amplitudePhaseChartCalculation.calculate(data, span);
        break;

    default:
        break;
    }

    return result;
}
