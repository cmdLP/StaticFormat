# TODO

- lazy formatting, save parameters in a struct, by reference if possible
    - write to stream directly when applying `operator<<` to format
    - write to stringstream in `operator std::string`
        - format with no parameters should yield string directly without streaming it to a stringstream
        
        
- formatting options
    - hexadecimal, binary, boolalpha...
    - alignment, fill character

    
- table formats, yields to multiple strings
    - `"{}|{}"_format(name, age)`
