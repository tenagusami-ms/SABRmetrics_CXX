//
// Created by ykanya on 2021/12/05.
//
#include "DataTable.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/iota.hpp>

auto csvLine2Items(const auto &line) {
    auto token_line{boost::tokenizer<boost::escaped_list_separator<char>>(line)
                    | boost::adaptors::transformed([](const auto &token) { return boost::trim_copy(token); })};
    return std::vector<std::string>{token_line.begin(), token_line.end()};
}

auto read_data_csv(const std::filesystem::path &file_path) -> TableHeaderBody {
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

template<typename DataType>
auto dataTableRowFirstIterators(
        const std::vector<DataType> &dataArray,
        const int nColumns) {
    return boost::range::iota(dataArray.size()) | boost::adaptors::transformed([&dataArray, nColumns](const int row) {
        return dataArray.begin() + nColumns + row;
    });
}

template<typename DataType>
struct DataTableView<DataType>::DataTableViewImpl {
    const std::vector<std::string> columnTitles;
    const std::vector<std::string> rowTitles;
    const std::vector<typename std::vector<DataType>::iterator> rowFirstIterators;
    const int nColumns;

    DataTableViewImpl() = delete;

    explicit DataTableViewImpl(const std::vector<DataType> &, int);

    explicit DataTableViewImpl(std::vector<typename std::vector<DataType>::iterator> &&, int);

    ~DataTableViewImpl() noexcept = delete;

    DataTableViewImpl(DataTableViewImpl &&) noexcept = default;

    DataTableViewImpl(const DataTableViewImpl &) = delete;

    DataTableViewImpl &operator=(DataTableViewImpl &&) = delete;

    DataTableViewImpl &operator=(const DataTableViewImpl &) = delete;
};

template<typename DataType>
DataTableView<DataType>::DataTableViewImpl::DataTableViewImpl(
        std::vector<typename std::vector<DataType>::iterator> &&rowIterators,
        const int nColumns)
        : rowFirstIterators{std::move(rowIterators)},
          nColumns{nColumns} {}

template<typename DataType>
DataTableView<DataType>::DataTableViewImpl::DataTableViewImpl(
        const std::vector<DataType> &dataArray,
        const int nColumns)
        : DataTableView<DataType>::DataTableViewImpl(dataTableRowFirstIterators(dataArray, nColumns), nColumns) {}

template<typename DataType>
DataTableView<DataType>::DataTableView(
        const std::vector<DataType> &dataArray,
        const int nColumns)
        : pImpl_{std::make_unique<DataTableView<DataType>::DataTableViewImpl>(dataArray, nColumns)} {}

template<typename DataType>
DataTableView<DataType>::~DataTableView() noexcept = default;

template<typename DataType>
DataTableView<DataType>::DataTableView(DataTableView<DataType> &&) noexcept = default;

template<typename DataType>
struct DataTable<DataType>::DataTableImpl {
    const std::vector<std::string> columnTitles;
    const std::vector<std::string> rowTitles;
    const std::vector<DataType> dataArray;
    const int nColumns;

    DataTableImpl() = delete;

    explicit DataTableImpl(std::vector<DataType> &&, int);

    ~DataTableImpl() noexcept = delete;

    DataTableImpl(DataTableImpl &&) noexcept = default;

    DataTableImpl(const DataTableImpl &) = delete;

    DataTableImpl &operator=(DataTableImpl &&) = delete;

    DataTableImpl &operator=(const DataTableImpl &) = delete;
};

template<typename DataType>
DataTable<DataType>::DataTableImpl::DataTableImpl(
        std::vector<DataType> &&dataArray,
        const int nColumns)
        : dataArray{std::move(dataArray)},
          nColumns{nColumns} {}

template<typename DataType>
DataTable<DataType>::DataTable(
        std::vector<DataType> &&dataArray,
        const int nColumns)
        : pImpl_{std::make_unique<DataTable<DataType>::DataTableImpl>(std::move(dataArray), nColumns)} {}

template<typename DataType>
DataTable<DataType>::~DataTable() noexcept = default;

template<typename DataType>
DataTable<DataType>::DataTable(DataTable<DataType> &&) noexcept = default;

