C++ 		= g++
STDCPP          = --std=c++11
ECHO		= echo

C++LINK		= $(C++)

OPTIMIZATION    =  -O2


LDLIBS          = -pthread


WARNINGS = -Wall -pedantic

C++FLAGS        = $(OPTIMIZATION)  $(STDCPP) $(WARNINGS)
LDFLAGS		= $(OPTIMIZATION) $(STDCPP)

.SUFFIXES:
.SUFFIXES: .cpp .o

.cpp.o:
	@$(ECHO)
	@$(ECHO) "Compiling Source File --" $<
	@$(ECHO) "---------------------"
	$(C++) $(C++FLAGS) -c $<
	@$(ECHO)



#
# If you want to add symbol table information for gdb/cachegrind
# specify dbg=1 on the "make" command line
ifeq ($(gdb), 1)
        LDFLAGS += -g
        C++FLAGS += -g
	OPTIMIZATION =-O0
endif
# --- Don't modify anything above this line --

# You may add code modules but do not remove any modules
# nor change their signature
# We will substitute our own versions of isPrime.cpp and Timer.cpp
# when we test your program, so any changes you make to them will be lost
#
POBJ	= Primes.o getInput.o prime_thrFn.o isPrime.o Timer.o
primes:	$(POBJ)
	$(C++LINK) $(LDFLAGS) -o $@ $(POBJ)  $(LDLIBS)



.PHONY: clean
clean:	
	$(RM) *.o  primes 
	$(RM) core

