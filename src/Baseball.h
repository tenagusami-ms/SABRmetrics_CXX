//
// Created by ykanya on 2021/12/05.
//

#ifndef SABRMETRICS_CXX_BASEBALL_H
#define SABRMETRICS_CXX_BASEBALL_H

#include <DataFrame/DataFrame.h>
#include <filesystem>

using StrDataFrame = hmdf::StdDataFrame<std::string>;

auto fieldingIndependentPitching(int, int, int, int) -> double;

StrDataFrame readDataTable(const std::filesystem::path &);

#endif //SABRMETRICS_CXX_BASEBALL_H
