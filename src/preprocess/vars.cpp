#include "vars.hpp"
#include <cstdio>
#include <regex>
#include <string>
#include <vector>

void Variable::addVariable(std::string name, std::string value) {
    this->vars.push_back({
        .name = name,
        .value = value,
        .preprocessed = ""
    });
}

std::vector<std::string> __getVariableMatches(std::string value) {
    std::regex variable_regex(R"((#\{.+?\}))");

    auto var_start = std::sregex_iterator(value.begin(), value.end(), variable_regex);
    auto var_end = std::sregex_iterator();

    std::vector<std::string> vars;

    for (std::sregex_iterator i = var_start; i != var_end; ++i) {
        vars.push_back(i->str());
    }

    return vars;
}

void __strReplace(std::string& main, std::string target, std::string replacer, bool untilNone) {
    int pos = main.find(target);
    do {
        if (pos == -1) break;
        main.replace(pos, target.length(), replacer);
        pos = main.find(target);
    } while(untilNone && pos != -1);
}

std::string __getVarName(std::string name) {
    std::string start = name.substr(2);
    return start.substr(0, start.length() - 1);
}

std::optional<std::string> Variable::preprocessValue(std::string name) {
    for(auto& var : this->vars) {
        if (var.name == name) {
            if (var.preprocessed.length() > 0) return var.value;
            std::string value = var.value;
            auto vars = __getVariableMatches(var.value);
            for (auto s : vars) {
                std::string str = __getVarName(s);
                __strReplace(value, s, this->preprocessValue(str).value_or(""), true);
            }

            var.preprocessed = value;
            return value;
        }
    }
    return {};
}
