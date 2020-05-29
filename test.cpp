// test FuzzyMatcher
//
// test regex text [max_error]
//
// where 0 <= max_error <= 255 is the maximum edit distance^*, should be small, default is 1
// *) Levenshstein distance: char insertion, substitution, deletion

// ./test 'abcd' 'axbcd'
// ./test 'abcd' 'axcd'
// ./test 'abcd' 'acd'
// ./test 'a😀cd' 'axcd'
// ./test 'abcd' 'a😀cd'
// ./test 'a\d+z' 'az'
// ./test 'a\d+z' 'a9'
// ./test 'ab_cd' 'abcd'
// ./test 'ab_cd' 'ab-cd'
// ./test 'ab_cd' 'abCd' 2

// #define DEBUG // enable debugging to stderr

#include "fuzzymatcher.h"

int main(int argc, char **argv)
{
  if (argc > 1)
  {
    try
    {
      std::string regex = reflex::Matcher::convert(argv[1], reflex::convert_flag::unicode);
      printf("\n** RE/flex converted regex = %s\n\n", regex.c_str());
      reflex::Pattern reflex_pattern(regex, "mr");
      if (argc > 2)
      {
        const char *text = argv[2];
        uint8_t max = 1;
        if (argc > 3)
        {
          unsigned long n = strtoul(argv[3], NULL, 10);
          if (n > 255)
          {
            fprintf(stderr, "max_error too large\n\n");
            exit(EXIT_FAILURE);
          }
          max = static_cast<uint8_t>(n);
        }
        reflex::FuzzyMatcher matcher(reflex_pattern, max, text);
        if (!matcher.matches())
          printf("No match\n");
        else
          printf("Match (%u edits)\n", matcher.edits());
        matcher.input(text);
        /* scan() will never work well with fuzzy matching, because of possible gaps i.e. abc matches abx but leaves x on the input
        while (matcher.scan())
          printf("Scan %zu '%s'\n", matcher.accept(), matcher.text());
        matcher.input(text);
          */
        while (matcher.find())
          printf("Find %zu '%s' at %zu,%zu spans %zu..%zu %s (%u edits)\n", matcher.accept(), matcher.text(), matcher.lineno(), matcher.columno(), matcher.first(), matcher.last(), matcher.at_end() ? "at end" : "", matcher.edits());
        matcher.input(text);
        while (matcher.split())
          printf("Split %zu '%s' at %zu (%u edits)\n", matcher.accept(), matcher.text(), matcher.columno(), matcher.edits());
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
