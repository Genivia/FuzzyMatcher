FuzzyMatcher
============

A C++ class extension of the [RE/flex](https://github.com/Genivia/RE-flex)
Matcher class to support fuzzy matching and search with regex patterns.

- specify max error as a parameter, i.e. the max edit distance or
  [Levenshstein distance](https://en.wikipedia.org/wiki/Levenshtein_distance)

- regex patterns are compiled into DFA VM opcodes for speed

- practically linear execution time in the length of the input, using
  DFA-based matching with minimal backtracking limited by the specified max
  error parameter

- supports the full RE/flex regex pattern syntax, which is POSIX-based with
  many additions: <https://www.genivia.com/doc/reflex/html/#reflex-patterns>

- no group captures (yet), except for top-level sub-pattern group captures,
  e.g. `(foo)|(bar)|(baz)` but not `(foo(bar))`

- Newlines and NUL characters are never deleted or substituted to ensure that
  errors (edits) do not result in fuzzy matches spanning multiple strings or
  lines

Examples
--------

pattern    | max | fuzzy `find()` matches            | but not
---------- | --- | --------------------------------- | -------------------------
`abc`      | 1   | `abc`, `ab`, `ac`, `axc`, `axbc`  | `a`, `axx`, `axbxc`, `bc`
`año`      | 1   | `año`, `ano`, `ao`                | `anno`
`ab_cd`    | 2   | `ab_cd`, `ab-cd`, `ab Cd`, `abCd` | `ab\ncd`, `Ab_cd`, `Abcd`
`a[0-9]+z` | 1   | `a1z`, `a123z`, `az`, `axz`       | `axxz`, `A123z`, `123z`

Note that the first character of the pattern must match when searching with the
`find()` method.  The `matches()` method does not impose this requirement:

pattern    | max | fuzzy `matches()` matches                            | but not
---------- | --- | ---------------------------------------------------- | -------------------------
`abc`      | 1   | `abc`, `ab`, `ac`, `Abc`, `xbc` `bc`, `axc`, `axbc`  | `a`, `axx`, `Ab`, `axbxc`
`año`      | 1   | `año`, `Año`, `ano`, `ao`, ``ño``                    | `anno`
`ab_cd`    | 2   | `ab_cd`, `Ab_Cd`, `ab-cd`, `ab Cd`, `Ab_cd`, `abCd`  | `ab\ncd`, `AbCd`
`a[0-9]+z` | 1   | `a1z`, `A1z`, `a123z`, `az`, `Az`, `axz`, `123z`     | `axxz`

Usage
-----

See the [RE/flex user guide](https://www.genivia.com/doc/reflex/html/#regex-methods)
for the full list of matcher class methods available to extract match info.

### Fuzzy searching

    #include "fuzzymatcher.h"

    reflex::FuzzyMatcher matcher("PATTERN", [max,] INPUT);

    while (matcher.find())
    {
      std::cout << matcher.text() << '\n' // show each fuzzy match
    }

where `max` is the maximum edit distance (1 by default) and `INPUT` is a
string, wide string, `FILE*`, or `std::istream` object.

### Fuzzy matching

    #include "fuzzymatcher.h"

    if (reflex::FuzzyMatcher("PATTERN", [max,] INPUT).matches())
    {
      std::cout << "fuzzy pattern matched\n";
    }

### Fuzzy splitting (text between matches)

    #include "fuzzymatcher.h"

    reflex::FuzzyMatcher matcher("PATTERN", [max,] INPUT);

    while (matcher.split())
    {
      std::cout << matcher.text() << '\n' // show text between fuzzy matches
    }

### Full Unicode support

To support full Unicode pattern matching, such as `\p` Unicode character
classes, convert the regex pattern before using it as follows:

    std::string regex(reflex::Matcher::convert("PATTERN", reflex::convert_flag::unicode));
    reflex::FuzzyMatcher matcher(regex, [max,] INPUT);

### Static regex patterns

Fixed patterns should be constructed (and Unicode converted) just once
statically to avoid repeated construction, e.g. in loops and function calls:

    static const reflex::Pattern pattern(reflex::Matcher::convert("PATTERN", reflex::convert_flag::unicode));

    if (reflex::FuzzyMatcher(pattern, [max,] INPUT).matches())
    {
      std::cout << "fuzzy pattern matched\n";
    }

Requires
--------

[RE/flex](https://github.com/Genivia/RE-flex) downloaded and locally build or
globally installed to access the `reflex/include` and `reflex/lib` files.

Build
-----

Assuming `reflex` dir with source code is locally build in the project dir:

    c++ -o myapp myapp.cpp -Ireflex/include reflex/lib/libreflex.a

When the `libreflex` library is installed:

    c++ -o myapp myapp.cpp -lreflex

Testing
-------

    $ make test
    $ ./test 'ab_cd' 'abCd' 2
    matches(): match (2 edits)
    find():    1 'abCd' at 1,0 spans 0..4 at end (2 edits)
    split():   1 '' at 0 (2 edits)
    split():   4294967295 '' at 4 (0 edits)

Bugs
----

This project is currently in beta.

License
-------

BSD-3
