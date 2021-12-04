#include <iostream>
#include <exception>
#include <filesystem>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace boost::property_tree;

#include <boost/tokenizer.hpp>

using BOOST_TOKENIZER_ESCAPED_LIST = boost::tokenizer<boost::escaped_list_separator<char>>;

int main() {
    const auto prefix_directory{std::filesystem::path{__FILE__}.parent_path()};
    const auto configuration_file{prefix_directory / "conf" / "config.json"};
    auto json{ptree{}};
    read_json(configuration_file, json);

    const auto spahn_file_path{std::filesystem::path(json.get_optional<std::string>("Spahn_file").get())};

    try {
        auto spahn_file{std::ifstream(spahn_file_path)};
        auto line{std::string{}};
        while (getline(spahn_file, line)) {
            BOOST_TOKENIZER_ESCAPED_LIST tokens1(line);
            std::for_each(tokens1.begin(), tokens1.end(),
                          [](const auto &token) {
                              std::cout << token << ",";
                          });
            std::cout << "\n";
        }
        spahn_file.close();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
