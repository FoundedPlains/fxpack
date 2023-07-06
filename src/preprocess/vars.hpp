#ifndef VARS_HPP
#define VARS_HPP

#include <map>
#include <optional>
#include <string>
#include <vector>

typedef struct {
    std::string name;
    std::string value;
    std::string preprocessed;
} VarStore;

class Variable {
    private:
        std::vector<VarStore> vars;

    public:
        void addVariable(std::string name, std::string value);
        std::optional<std::string> preprocessValue(std::string name);
};

#endif
