CXX       = c++
REFLEX    = ../bin/reflex
REFLAGS   =
LIBREFLEX =../lib/libreflex.a
CXXOFLAGS = -O2
CXXWFLAGS = -Wall -Wunused -Wextra
CXXIFLAGS = -I. -I../include
CXXMFLAGS =
CXXFLAGS  = $(CXXWFLAGS) $(CXXOFLAGS) $(CXXIFLAGS) $(CXXMFLAGS)

ftest:		ftest.cpp fuzzymatcher.h
		$(CXX) $(CXXFLAGS) -o $@ $< $(LIBREFLEX)
		./ftest

.PHONY:		clean

clean:
		-rm -rf *.dSYM
		-rm -f *.o *.gch *.log
		-rm -f lex.yy.h lex.yy.cpp y.tab.h y.tab.c
		-rm -rf *.dSYM
		-rm -f *.o *.gch *.log
		-rm -f lex.yy.h lex.yy.cpp y.tab.h y.tab.c reflex.*.cpp reflex.*.gv reflex.*.txt
		-rm -f a.out dump.gv dump.pdf dump.cpp
		-rm -f ftest
