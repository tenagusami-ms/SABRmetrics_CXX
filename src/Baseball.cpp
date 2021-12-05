//
// Created by ykanya on 2021/12/05.
//

#include "Baseball.h"

auto fieldingIndependentPitching(
        const int homeRuns,
        const int basesOnBalls,
        const int strikeOuts,
        const int inningsPitched) -> double {
    return static_cast<double>(13 * homeRuns + 3 * basesOnBalls - 2 * strikeOuts)
           / static_cast<double>((inningsPitched));
}
