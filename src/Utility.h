//
// Created by ykanya on 2021/12/05.
//
#pragma once
#ifndef SABRMETRICS_CXX_UTILITY_H
#define SABRMETRICS_CXX_UTILITY_H

#include <tuple>
#include <vector>
#include <string>
#include <filesystem>

using CSVHeader = std::vector<std::string>;
using CSVBody = std::vector<std::vector<std::string>>;
using CSVHeaderBody = std::tuple<CSVHeader, CSVBody>;

CSVHeaderBody read_data_csv(const std::filesystem::path &);

#endif //SABRMETRICS_CXX_UTILITY_H
