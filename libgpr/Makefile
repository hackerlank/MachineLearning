INCPATH		= -I ../../ulib/include -I ../../gflags/include

EXTRALIB	?=

CXXFLAGS	?= -O3 -g3 -fPIC -W -Wall
DEBUG		?=

GPR_OBJS	= $(addsuffix .o, $(basename $(wildcard src/*.cpp)))
GPR		= lib/libgpr.a

.cpp.o:
	@echo -e "  CXX\t$@"
	@$(CXX) $(INCPATH) $(CXXFLAGS) -c -o "$@" "$<"

all: $(GPR)

$(GPR): $(GPR_OBJS)
	@echo -e "  AR\t$(GPR)"	
	@$(AR) csr $(GPR) $(GPR_OBJS)
	@cp src/*.h include/

clean:
	@rm -rf $(GPR) $(GPR_OBJS) include/*
	@find . -name "*~" | xargs rm -rf

.PHONY: all clean
