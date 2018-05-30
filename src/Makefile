PROJECT=safe
MYLIB=
SYSLIB=-lpthread
CFLAG=-g
CC=gcc
GCC=gcc
OBJDIR=./obj
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

$(PROJECT):$(OBJS)
	$(GCC) -o $@ $^ $(CFLAG) $(SYSLIB) $(MYLIB)
$(OBJDIR)/%.o:%.c
	$(GCC) -c -o $@ $<


clean:
	rm -f $(OBJDIR)/*.o $(PROJECT)