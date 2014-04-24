PUB	= /home/linux/ieng6/cs100s/public
include $(PUB)/Arch/arch.gnu-4.7
WARNINGS += -Wall -pedantic

 
VEC_OBJ	= vector.o Timer.o

vector:	        $(VEC_OBJ)
		$(C++LINK) $(LDFLAGS) -o $@ $(VEC_OBJ)  $(LDLIBS)


.PHONY: clean
clean:
	$(RM) *.o vector
	$(RM) core
