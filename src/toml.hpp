#ifndef TOML_HPP
#define TOML_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <toml.h>

namespace toml {
    enum class TomlTypes {
        TOML_STRING,
        TOML_INT,
        TOML_FLOAT,
        TOML_BOOL,
        TOML_ARRAY,
        TOML_TABLE,
        TOML_DATETIME,
        TOML_NONE
    };

    class TomlNode {
        public:
            TomlTypes type;
            std::string key;
            std::string errBuff;
            struct {
                std::string s;
                int64_t i;
                double f;
                bool b;
                std::vector<std::shared_ptr<TomlNode>> a;
                std::map<std::string_view, std::shared_ptr<TomlNode>> t;
                toml_timestamp_t d;
            } value;
    };

    std::shared_ptr<TomlNode> parse(char *toml);
}

#endif
