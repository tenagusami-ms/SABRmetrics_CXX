#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "src/Spahn.h"

using namespace boost::property_tree;

int main() {
    const auto prefix_directory{std::filesystem::path{__FILE__}.parent_path()};
    const auto configuration_file{prefix_directory / "conf" / "config.json"};
    auto json{ptree{}};
    read_json(configuration_file, json);
    process_spahn(json);

    return 0;
}
