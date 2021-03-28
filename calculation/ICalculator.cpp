#include "ICalculator.hpp"
#include <limits>

using Calculation::ICalculator;

qreal ICalculator::diracDelta(qreal timePoint) const
{
    if(timePoint != 0)
    {
        return 0;
    }
    else
    {
        return std::numeric_limits<std::uint32_t>::max();
    }
}
