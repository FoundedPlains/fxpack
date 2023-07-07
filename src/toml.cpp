#include "toml.hpp"
#include <memory>

namespace toml {
    std::shared_ptr<TomlNode> handleRaw(const char *raw);
    std::shared_ptr<TomlNode> handleTable(toml_table_t *table);
    std::shared_ptr<TomlNode> handleArray(toml_array_t *arr);

    std::shared_ptr<TomlNode> handleRaw(const char *raw) {
        using enum TomlTypes;
        auto node = std::make_shared<TomlNode>();
        char *sval;
        int64_t ival;
        int bval;
        double dval;
        toml_timestamp_t ts;
        char dbuf[100];

        if (!toml_rtos(raw, &sval)) {
            node->type = TOML_STRING;
            node->value.s = sval;
        } else if (!toml_rtoi(raw, &ival)) {
            node->type = TOML_INT;
            node->value.i = ival;
        } else if (!toml_rtob(raw, &bval)) {
            node->type = TOML_BOOL;
            node->value.b = bval;
        } else if (!toml_rtod_ex(raw, &dval, dbuf, sizeof(dbuf))) {
            node->type = TOML_FLOAT;
            node->value.f = dval;
        } else if (!toml_rtots(raw, &ts)) {
            node->type = TOML_DATETIME;
            node->value.d = ts;
        } else {
            node->type = TOML_NONE;
        }

        return node;
    }

    std::shared_ptr<TomlNode> handleArray(toml_array_t *arr) {
        auto node = std::make_shared<TomlNode>();
        node->type = TomlTypes::TOML_ARRAY;
        const char *raw;
        toml_array_t *arr2;
        toml_table_t *tab;

        if (toml_array_kind(arr) == 't') {
            for (int i = 0; (tab = toml_table_at(arr, i)); i++) {
                auto tmp = handleTable(tab);
                node->value.a.push_back(std::move(tmp));
            }
            return node;
        }

        switch (toml_array_kind(arr)) {
            case 'v':
                for (int i = 0; (raw = toml_raw_at(arr, i)); i++) {
                    node->value.a.push_back(handleRaw(raw));
                }
                break;
            case 'a':
                for (int i = 0; (arr2 = toml_array_at(arr, i)); i++) {
                    node->value.a.push_back(handleArray(arr2));
                }
                break;
            default:
                break;
        }

        return node;
    }

    std::shared_ptr<TomlNode> handleTable(toml_table_t *table) {
        auto node = std::make_shared<TomlNode>();
        node->type = TomlTypes::TOML_TABLE;

        const char *key;
        const char *raw;
        toml_array_t *arr;
        toml_table_t *tab;

        for (int i = 0; (key = toml_key_in(table, i)); i++) {
            if ((raw = toml_raw_in(table, key))) {
                auto tmp = handleRaw(raw);
                tmp->key = key;
                if (node->type == TomlTypes::TOML_TABLE) {
                    node->value.t.try_emplace(key, std::move(tmp));
                } else {
                    node->value.a.push_back(std::move(tmp));
                }
            } else if ((arr = toml_array_in(table, key))) {
                auto tmp = handleArray(arr);
                tmp->key = key;
                if (node->type == TomlTypes::TOML_TABLE) {
                    node->value.t.try_emplace(key, std::move(tmp));
                } else {
                    node->value.a.push_back(std::move(tmp));
                }
            } else if ((tab = toml_table_in(table, key))) {
                auto tmp = handleTable(tab);
                tmp->key = key;
                if (node->type == TomlTypes::TOML_TABLE) {
                    node->value.t.try_emplace(key, std::move(tmp));
                } else {
                    node->value.a.push_back(std::move(tmp));
                }
            }
        }

        return node;
    }

    std::shared_ptr<TomlNode> parse(char* toml) {
        char errBuff[200];
        toml_table_t *tbl = toml_parse(toml, errBuff, sizeof(errBuff));
        if (!tbl) {
            auto node = std::make_shared<TomlNode>();
            node->errBuff = errBuff;
            node->type = TomlTypes::TOML_NONE;
            return node;
        }

        return handleTable(tbl);
    }
}
