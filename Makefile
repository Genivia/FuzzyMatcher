CXX       = c++
REFLEX    = ../bin/reflex
REFLAGS   =
LIBREFLEX =../lib/libreflex.a
YACC      = bison -y
INCPCRE2  = /opt/local/include
LIBPCRE2  = -L/opt/local/lib -lpcre2-8
INCBOOST  = /opt/local/include
LIBBOOST  = -L/opt/local/lib -lboost_regex-mt
CXXOFLAGS = -O2
CXXWFLAGS = -Wall -Wunused -Wextra
CXXIFLAGS = -I. -I../include -I $(INCPCRE2) -I $(INCBOOST)
CXXMFLAGS =
CXXFLAGS  = $(CXXWFLAGS) $(CXXOFLAGS) $(CXXIFLAGS) $(CXXMFLAGS)

test:		test.cpp fuzzymatcher.h
		$(CXX) $(CXXFLAGS) -o $@ $< $(LIBREFLEX) $(LIBPCRE2) $(LIBBOOST)

.PHONY:		clean

clean:
		-rm -rf *.dSYM
		-rm -f *.o *.gch *.log
		-rm -f lex.yy.h lex.yy.cpp y.tab.h y.tab.c
		-rm -rf *.dSYM
		-rm -f *.o *.gch *.log
		-rm -f lex.yy.h lex.yy.cpp y.tab.h y.tab.c reflex.*.cpp reflex.*.gv reflex.*.txt
		-rm -f a.out test_regex_history dump.gv dump.pdf dump.cpp
		-rm -f test
