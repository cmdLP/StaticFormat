# StaticFormat
A simple formatting library. The format-string is parsed at compile-time by using template-meta-programming, to guarantee the best performance. No need to do extra work at runtime. It uses a GNU-extension to achieve this, it works with GCC and Clang.

```
#include <iostream>
#include "static_format.hpp"

using namespace static_format::literals;

int main() {
    std::cout << "Hello {}!\n"_format("world");
    std::cout << "int main() {{ return {}; }};\n"_format(0);
    std::cout << "x = {} = 0x{}{}\n"_format(42, std::hex, 42);
};
```

This gets transformed to something like this:

```
#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::stringstream ss0;
    ss0 << "Hello " << "world" << "!\n";
    std::cout << ss0.str();
    /// end scope: ss0
    
    std::stringstream ss1;
    ss1 << "int main() { return " << 0 << "; };\n";
    std::cout << ss1.str();
    /// end scope: ss1
    
    std::stringstream ss2;
    ss2 << "x = " << 42 << " = 0x" << std::hex << 42 << '\n';
    std::cout << ss2.str();
    /// end scope: ss2
};
```