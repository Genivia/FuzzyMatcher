FuzzyMatcher
============

A C++ class extension of the [RE/flex](https://github.com/Genivia/RE-flex)
library for fuzzy matching with regex patterns.

- supports RE/flex regex pattern syntax, i.e. POSIX based, but with many additions
- regex patterns are compiled to the RE/flex DFA VM opcodes for speed
- specify max edit distance ([Levenshstein distance](https://en.wikipedia.org/wiki/Levenshtein_distance)) as a parameter
- practically linear execution time in the length of the input N; pathological worst case O(NM) time for regex length M
- no group captures (yet), except for top-level sub-pattern captures e.g. `(foo)|(bar)|(baz)`

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

To convert the regex pattern containing Unicode, such as `\p` Unicode classes:

    std::string regex reflex::Matcher::convert("PATTERN", reflex::convert_flag::unicode);
    reflex::FuzzyMatcher matcher(regex, [max,] INPUT);

Examples
--------

pattern    | max | matches                           | but not
---------- | --- | --------------------------------- | ---------------------------
`abc`      | 1   | `abc`, `ab`, `ac`, `axc`, `axbc`  | `a`, `axx`, `axbxc`, `bc`
`año`      | 1   | `año`, `ano`, `ao`                | `anno`
`ab_cd`    | 2   | `ab_cd`, `ab-cd`, `ab Cd`, `abCd` | `ab\ncd`, `Ab_cd`, `Abcd`
`a[0-9]+z` | 1   | `a1z`, `a123z`, `az`, `axz`       | `axxz`, `A123z`, `123z`

Note that the first character of the pattern must match when searching text
with the `find()` method.  Newlines and NUL characters are not deleted or
substituted to ensure that matches are constrained within single strings and
lines.

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

BSD-3
