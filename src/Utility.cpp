//
// Created by ykanya on 2021/12/05.
//
#include "Utility.h"
#include <fstream>
#include <filesystem>
#include <boost/tokenizer.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string.hpp>

auto csvLine2Items(const auto &line) {
    auto token_line{boost::tokenizer<boost::escaped_list_separator<char>>(line)
                    | boost::adaptors::transformed([](const auto &token) { return boost::trim_copy(token); })};
    return std::vector<std::string>{token_line.begin(), token_line.end()};
}

CSVHeaderBody read_data_csv(const std::filesystem::path &file_path) {
    auto spahn_file{std::ifstream(file_path)};
    auto line{std::string{}};
    std::getline(spahn_file, line);
    const auto titleItems{csvLine2Items(line)};

    auto valueTableString{std::vector<std::vector<std::string>>{}};
    while (std::getline(spahn_file, line)) {
        valueTableString.emplace_back(csvLine2Items(line));
    }
    spahn_file.close();
    return std::make_tuple(titleItems, valueTableString);
}
