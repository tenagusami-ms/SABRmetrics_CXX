//
// Created by ykanya on 2021/12/05.
//
#pragma once
#ifndef SABRMETRICS_CXX_DATATABLE_H
#define SABRMETRICS_CXX_DATATABLE_H

#include <tuple>
#include <vector>
#include <string>
#include <filesystem>

using TableHeader = std::vector<std::string>;
using TableBody = std::vector<std::vector<std::string>>;
using TableHeaderBody = std::tuple<TableHeader, TableBody>;

template<typename DataType>
struct DataTableView {
    struct DataTableViewImpl;
    const std::unique_ptr<DataTableViewImpl> pImpl_;

    DataTableView() = delete;

    explicit DataTableView(const std::vector<DataType> &, int);

    ~DataTableView() noexcept;  /// 宣言の段階では不完全型のpImplにunique_ptrを使用しているので、ここでは宣言だけで、定義してはいけない。
    DataTableView(DataTableView<DataType> &&) noexcept;  /// 宣言の段階では不完全型のpImplにunique_ptrを使用しているので、ここでは宣言だけで、定義してはいけない。
    DataTableView(const DataTableView<DataType> &) = delete;

    DataTableView &operator=(DataTableView<DataType> &&) = delete;

    DataTableView &operator=(const DataTableView<DataType> &) = delete;
};

template<typename DataType>
struct DataTable {
    struct DataTableImpl;
    const std::unique_ptr<DataTableImpl> pImpl_;

    DataTable() = delete;

    explicit DataTable(std::vector<DataType> &&, int);

    ~DataTable() noexcept;  /// 宣言の段階では不完全型のpImplにunique_ptrを使用しているので、ここでは宣言だけで、定義してはいけない。
    DataTable(DataTable<DataType> &&) noexcept;  /// 宣言の段階では不完全型のpImplにunique_ptrを使用しているので、ここでは宣言だけで、定義してはいけない。
    DataTable(const DataTable<DataType> &) = delete;

    DataTable &operator=(DataTable<DataType> &&) = delete;

    DataTable &operator=(const DataTable<DataType> &) = delete;
};


auto read_data_csv(const std::filesystem::path &) -> TableHeaderBody;

#endif //SABRMETRICS_CXX_DATATABLE_H
