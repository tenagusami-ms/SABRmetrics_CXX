#include <filesystem>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "src/Spahn.h"
#include "src/Teams.h"

using namespace boost::property_tree;

int main() {
    const auto prefix_directory{std::filesystem::path{__FILE__}.parent_path()};
    auto json{ptree{}};
    read_json(prefix_directory / "conf" / "config.json", json);

    process_spahn(json);
    processTeams();

    return 0;
}
