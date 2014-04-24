PUB	= /home/linux/ieng6/cs100s/public
include $(PUB)/Arch/arch.gnu-4.7
WARNINGS += -Wall -pedantic


HELLO_OBJ	= hello_th.o

hello_th:	$(HELLO_OBJ)
		$(C++LINK) $(LDFLAGS) -o $@ $(HELLO_OBJ)  $(LDLIBS)

.PHONY: clean
clean:	
	$(RM) *.o hello_th
	$(RM) core
