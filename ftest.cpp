// test FuzzyMatcher
//
// test regex text [max_error [ids]]
//
// where:
// 0 <= max_error <= 255 is the maximum edit distance^*, should be small, defaults to 1
// ids is one or more characters i, d, or s to limit edits to insert/delete/substitute
//
// *) Levenshstein distance: char insertion, substitution, deletion
//
// Build after installing RE/flex:
//   c++ -o test test.cpp -lreflex
//
// Run:
//   ./test 'abcd' 'axbcd'
//   ./test 'abcd' 'axcd'
//   ./test 'abcd' 'acd'
//   ./test 'a😀cd' 'axcd'
//   ./test 'abcd' 'a😀cd'
//   ./test 'a\d+z' 'az'
//   ./test 'a\d+z' 'a9'
//   ./test 'ab_cd' 'abcd'
//   ./test 'ab_cd' 'ab-cd'
//   ./test 'ab_cd' 'abCd' 2

// #define DEBUG // enable debugging to stderr

#include "fuzzymatcher.h"

int main(int argc, char **argv)
{
  if (argc > 1)
  {
    try
    {
      std::string regex = reflex::Matcher::convert(argv[1], reflex::convert_flag::unicode);
      reflex::Pattern reflex_pattern(regex, "mr");
      if (argc > 2)
      {
        const char *text = argv[2];
        uint8_t max = 1;
        uint16_t flags = 0;
        if (argc > 3)
        {
          unsigned long n = strtoul(argv[3], NULL, 10);
          if (n > 255)
          {
            fprintf(stderr, "max_error too large\n\n");
            exit(EXIT_FAILURE);
          }
          max = static_cast<uint8_t>(n);
          if (argc > 4)
          {
            if (strchr(argv[4], 'i') != NULL)
              flags |= reflex::FuzzyMatcher::INS;
            if (strchr(argv[4], 'd') != NULL)
              flags |= reflex::FuzzyMatcher::DEL;
            if (strchr(argv[4], 's') != NULL)
              flags |= reflex::FuzzyMatcher::SUB;
          }
        }
        reflex::FuzzyMatcher matcher(reflex_pattern, max | flags, text);
        if (!matcher.matches())
          printf("matches(): no match\n");
        else
          printf("matches(): match (%u edits)\n", matcher.edits());
        matcher.input(text);
        /* scan() may not work well with fuzzy matching, because of possible "gaps" between matches caused by fuzzy matches
        while (matcher.scan())
          printf("scan():    '%s' at %zu (%u edits)\n", matcher.text(), matcher.columno(), matcher.edits());
        matcher.input(text);
          */
        while (matcher.find())
          printf("find():    '%s' at %zu (%u edits)\n", matcher.text(), matcher.columno(), matcher.edits());
        matcher.input(text);
        while (matcher.split())
          printf("split():   '%s' at %zu (%u edits)\n", matcher.text(), matcher.columno(), matcher.edits());
      }
    }
    catch (const reflex::regex_error& e)
    {
      std::cerr << e.what();
    }
  }
  else
  {
    fprintf(stderr, "Usage: test regex text [max_error]\n\n");
  }
  return 0;
}
