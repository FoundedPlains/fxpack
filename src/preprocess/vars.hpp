#ifndef VARS_HPP
#define VARS_HPP

#include <map>
#include <variant>
#include <string>
#include <vector>

typedef struct {
    std::string name;
    std::string value;
    std::string preprocessed;
} VarStore;

typedef struct {
    std::string msg;
} VarError;

class Variable {
    private:
        std::vector<VarStore> vars;

    public:
        void addVariable(std::string name, std::string value);
        std::variant<std::string, VarError> preprocessValue(std::string name,
            std::vector<std::string> names = {}
        );
};

#endif
