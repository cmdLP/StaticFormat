/**\
**
**  Copyright cmdLP
**
**  read LICENSE.md
**
**  test the formatting library
**
\**/

#include <iostream>
#include "static_format.hpp"

using namespace static_format::literals;

int main(int argc, char const* const* argv) {
    std::cout << "Hello {}!\n"_format("world");
    std::cout << R"(Use {{}} to {} a par{}meter into the formatted {}.
)"_format("insert", 'a', "text");

    std::cout << "a{} single char\n{}{}no char\n"_format("", "", "");

    /// utf-8 encoding:
    std::cout << "Hällö {}!\n"_format("☺☻");
    //std::wcout << L"Hällö {}!\n"_format(L"☺");

};
