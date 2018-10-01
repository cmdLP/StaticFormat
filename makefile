 ###
##
##  Copyright cmdLP
##
##  read LICENSE.md
##
##  static_format makefile
##
 ###

test:   test.cpp static_format.hpp
	$(CXX) -std=gnu++14 -Wno-gnu-string-literal-operator-template -Wno-gnu -O3 test.cpp


test.s:	test.cpp static_format.hpp
	$(CXX) -std=gnu++14 -Wno-gnu-string-literal-operator-template -Wno-gnu -O3 test.cpp -S
