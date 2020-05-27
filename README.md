FuzzyMatcher
============

A C++ class extension of the [RE/flex](https://github.com/Genivia/RE-flex)
library for fuzzy matching with regex patterns.

- supports RE/flex regex pattern syntax, i.e. POSIX based, but with extensions
- regex patterns are compiled to the RE/flex FSM VM opcodes for efficiency
- max edit distance (Levenshstein distance) is specified as a parameter
- worst case O(N M) time with regex length N and text length M (assuming a small max edit distance)

Usage
-----

    #include <fuzzymatcher.h>

    reflex::FuzzyMatcher matcher("PATTERN", [max,] INPUT);

    while (matcher.find())
    {
      ... report fuzzy match ...
    }

where `max` is the maximum edit distance or 1 by default and `INPUT` is a
string, wide string, `FILE*`, or `std::istream` object.

Optionally, convert the regex pattern as follows to fully support Unicode:

    #include <fuzzymatcher.h>

    std::string regex reflex::Matcher::convert("PATTERN", reflex::convert_flag::unicode);
    reflex::FuzzyMatcher matcher(regex, [max,] INPUT);

Requires
--------

[RE/flex](https://github.com/Genivia/RE-flex) locally or installed to access
the `reflex/include` and `reflix/lib` files.

Build
-----

Assuming `reflex` dir with source code is locally placed in the project dir:

    c++ -o myapp myapp.cpp -Ireflex/include reflex/lib/libreflex.a

When the `libreflex` library is installed:

    c++ -o myapp myapp.cpp -lreflex

Testing
-------

    make test
    ./test 'ab_cd' 'abCd' 2

Bugs
----

This is currently in beta.

License
-------

BSD-3.
