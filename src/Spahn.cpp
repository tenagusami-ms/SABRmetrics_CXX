//
// Created by ykanya on 2021/12/05.
//
// 投手データ
// 0 year: i32,  // 年
// 1 age: i32,  // 年齢
// 2 team: String,  // チーム
// 3 league: String,  // リーグ
// 4 wins: i32,  // 勝利数(W)
// 5 losses: i32,  // 敗戦数(L)
// 6 win_rate: f64, // 勝率(W.L)
// 7 earned_run_average: f64,  // 防御率(ERA)
// 8 games: i32,  // 試合数(G)
// 9 games_started: i32,  // 先発数(GS)
// 10 games_finished: i32,  // 先発を除き、ゲームの最後の投手になった試合数(GF)
// 11 complete_games: i32,  // 完投数(CG)
// 12 shutouts: i32, //完封数(SHO)
// 13 game_saves_as_a_relief_pitcher: i32,  // セーブ数(SV)
// 14 innings_pitched: f64, // 投球回数(IP)
// 15 hits: i32,  // 被安打数(H)
// 16 runs_allowed: i32,  // 失点(R)
// 17 earned_runs: i32,  // 自責点(ER)
// 18 home_runs: i32,  // 被本塁打数(HR)
// 19 bases_on_balls: i32,  // 与四球数(BB)
// 20 intentional_walks: i32,  // 敬遠による与四球数(IBB)
// 21 strike_outs: i32, // 奪三振数(SO)
// 22 hits_by_pitch: i32,  // 与死球数(HBP)
// 23 balks: i32,  // ボーク数(BK)
// 24 wild_pitches: i32,  // 暴投数(WP)
// 25 batters_faced: i32,  // 対戦打者数(BF)
// 26 adjusted_earned_run_average_plus: f64,  // 修正防御率(ERA+)
// 27 walks_and_hits_per_innings_pitched: f64,  // 1イニング当たりに投手が許した走者の数(WHIP)
// 28 hits_per_nine_innings: f64,  // 9イニングあたり被安打率(H/9)
// 29 home_runs_per_nine_innings: f64,  // 9イニングあたり被本塁打率(HR/9)
// 30 walks_per_nine_innings: f64,  // 9イニングあたり与四球率(BB/9)
// 31 strikeouts_per_nine_innings: f64,  // 9イニングあたり奪三振率(SO/9)
// 32 strikeout_walk_ratio: f64,  // 三振と四球の割合(SO/BB)
// 33awards: String,  // 受賞
#include "Spahn.h"
#include "Baseball.h"
#include <iostream>
#include <filesystem>
#include <functional>
#include "DataTable.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/copied.hpp>
#include <boost/range/algorithm/sort.hpp>

using namespace boost::property_tree;
using namespace boost::accumulators;
using accumulator_t = accumulator_set<double, stats<tag::p_square_quantile>>;


void dumpTable(const TableHeaderBody &spahnCSV) {
    const auto[spahnTitles, spahnTable]{spahnCSV};
    std::cout << boost::join(spahnTitles, "\t") << "\n";
    boost::for_each(spahnTable, [](const auto &line) {
        std::cout << boost::join(line, "\t") << "\n";
    });
}

void statisticsEarnedRunAverages(const TableBody &spahnTable) {
    const auto earnedRunAverages{
            spahnTable | boost::adaptors::transformed([](const auto &line) { return std::stod(line.at(7)); })};
    accumulator_set<double, stats<tag::median> > accMedian;
    auto acc1stQuartile{accumulator_t(quantile_probability = 0.25)};
    auto acc3rdQuartile{accumulator_t(quantile_probability = 0.75)};
    boost::for_each(earnedRunAverages, [&accMedian, &acc1stQuartile, &acc3rdQuartile](const auto value) {
        accMedian(value);
        acc1stQuartile(value);
        acc3rdQuartile(value);
    });
    const double minimumERA{*boost::range::min_element(earnedRunAverages)};
    std::cout << "min: " << *boost::range::min_element(earnedRunAverages) << "\n";
    std::cout << "1st quartile: " << p_square_quantile(acc1stQuartile) << "\n";
    std::cout << "median: " << extract::median(accMedian) << "\n";
    std::cout << "3rd quartile: " << p_square_quantile(acc3rdQuartile) << "\n";
    std::cout << "max: " << *boost::range::max_element(earnedRunAverages) << "\n";
    std::cout << "age with minimum ERA: "
              << *((spahnTable | boost::adaptors::filtered(
                      [minimumERA](const auto &line) { return std::stod(line.at(7)) == minimumERA; }))
                           .begin()->begin() + 1) << "\n";
}

auto annualData2FieldingIndependentPitching(const TableBody::value_type &annualData) {
    return fieldingIndependentPitching(
            std::stoi(annualData.at(18)), std::stoi(annualData.at(19)),
            std::stoi(annualData.at(21)), std::stoi(annualData.at(14)));
}

auto FIPSorted(const TableBody &table, const std::function<double(TableBody::value_type)> &annualQuantity) {
    auto tableCopied{table | boost::adaptors::copied(0, table.size())};
    boost::range::sort(tableCopied, [&annualQuantity](const auto &year_data1, const auto &year_data2) {
        return annualQuantity(year_data1) < annualQuantity(year_data2);
    });
    return tableCopied;
}

void statisticsFIP(const TableHeaderBody &spahnCSV) {
    const auto&[spahnTitles, spahnTable]{spahnCSV};
    const auto spahnFIPSorted{FIPSorted(spahnTable, annualData2FieldingIndependentPitching)};
    const auto columns{std::vector<int>{0, 1, 4, 5, 7}};
    const auto titles{columns | boost::adaptors::transformed([&spahnTitles](const int index) {
        return spahnTitles.at(index);
    })};
    std::cout << boost::join(titles, "\t") << "\t" << "FIP" << "\n";
    for (auto &annualData: spahnFIPSorted) {
        const auto annuals{columns | boost::adaptors::transformed([&annualData](const int index) {
            return annualData.at(index);
        })};
        std::cout << boost::join(annuals, "\t") << "\t" << annualData2FieldingIndependentPitching(annualData) << "\n";
    }
}

void process_spahn(ptree &json) {
    const auto spahnCSV{read_data_csv(std::filesystem::path(json.get_optional<std::string>("Spahn_file").get()))};

    dumpTable(spahnCSV);
    statisticsEarnedRunAverages(std::get<1>(spahnCSV));
    statisticsFIP(spahnCSV);
}
