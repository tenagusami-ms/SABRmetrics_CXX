#include <iostream>
#include <exception>
#include <filesystem>
#include <vector>
#include<string>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>

using namespace boost::property_tree;
using namespace boost::accumulators;
using accumulator_t = accumulator_set<double, stats<tag::p_square_quantile>>;

auto csvLine2Items(const auto &line) {
    auto token_line{boost::tokenizer<boost::escaped_list_separator<char>>(line)
                    | boost::adaptors::transformed([](const auto &token) { return boost::trim_copy(token); })};
    return std::vector<std::string>{token_line.begin(), token_line.end()};
}

auto read_data_csv(const auto &file_path) {
    auto valueTableString{std::vector<std::vector<std::string>>{}};
    auto titleItems{std::vector<std::string>{}};
    try {
        auto spahn_file{std::ifstream(file_path)};
        auto line{std::string{}};
        std::getline(spahn_file, line);
        titleItems = csvLine2Items(line);
        while (std::getline(spahn_file, line)) {
            valueTableString.emplace_back(csvLine2Items(line));
        }
        spahn_file.close();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return std::make_tuple(titleItems, valueTableString);

}

void process_spahn(ptree &json) {
    const auto[spahn_titles, spahn_csv]{
            read_data_csv(std::filesystem::path(json.get_optional<std::string>("Spahn_file").get()))};
    boost::for_each(spahn_csv, [](const auto &line) {
        std::cout << boost::join(line, "\t") << "\n";
    });

    accumulator_set<double, stats<tag::median> > accMedian;
    auto acc1stQuartile{accumulator_t(quantile_probability = 0.25)};
    auto acc3rdQuartile{accumulator_t(quantile_probability = 0.75)};
    boost::for_each(spahn_csv, [&accMedian, &acc1stQuartile, &acc3rdQuartile](const auto line) {
        const auto value{std::stod(line[7])};
        accMedian(value);
        acc1stQuartile(value);
        acc3rdQuartile(value);
    });
    std::cout << "1st quartile: " << p_square_quantile(acc1stQuartile) << "\n";
    std::cout << "median: " << extract::median(accMedian) << "\n";
    std::cout << "3rd quartile: " << p_square_quantile(acc3rdQuartile) << "\n";
}


int main() {
    const auto prefix_directory{std::filesystem::path{__FILE__}.parent_path()};
    const auto configuration_file{prefix_directory / "conf" / "config.json"};
    auto json{ptree{}};
    read_json(configuration_file, json);
    process_spahn(json);

    return 0;
}
