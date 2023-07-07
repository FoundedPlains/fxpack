#include "utils.hpp"
#include "toml.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#define VERSION "0.0.1"

void build() {
    std::string file = readFile("fxbuild.toml");
    if (file.length() == 0) {
        fprintf(stderr, "File \"fxbuild.toml\" not found or is empty\n");
        return;
    }

    auto config = toml::parse(file.data());
    if (config->type == toml::TomlTypes::TOML_TABLE) {
        for (const auto& [key, value] : config->value.t) {
        }
    }
}

void printHelp() {
    printf(R"(fxpack - Simple package manager in C++
Usage: fxpack [ACTION] [OPTIONS] (ARGS)

ACTION:
  help, -h          Display this help message
  version, -v       Display version
  build             Build a package from fxbuild.toml
  install (NAME)    Install (NAME) package
                      Use  @VER to specify version
                      Like this fxpack install linux@6.4.2
)");
}

int main(int argc, char **argv) {

    // ARG handling
    if (argc == 1) {
        printHelp();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        char *s = argv[i];
        if (!strcmp(s, "-v") || !strcmp(s, "version")) {
            printf(VERSION);
            break;
        } else if (!strcmp(s, "build")) {
            build();
        }
    }

    return EXIT_SUCCESS;
}
