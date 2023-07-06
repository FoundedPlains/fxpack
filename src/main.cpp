#include <cstdio>
#include <cstdlib>
#include <tuple>
#include <variant>
#include "preprocess/vars.hpp"

void handle(std::variant<std::string, VarError>& ret) {
    std::string *v = std::get_if<std::string>(&ret);
    if (!v) {
        VarError err = std::get<VarError>(ret);
        fprintf(stderr, "%s\n", err.msg.c_str());
    } else {
        printf("%s\n", v->c_str());
    }

}

int main(int argc, char **argv) {
    Variable var;
    var.addVariable("xd", "lol #{xd}");

    auto ret = var.preprocessValue("xd");
    handle(ret);
    var.addVariable("xd", "lol #{nice}");
    ret = var.preprocessValue("xd");
    handle(ret);
    var.addVariable("nice", "works");
    ret = var.preprocessValue("xd");
    handle(ret);

    return EXIT_SUCCESS;
}
