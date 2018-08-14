SHELL = /bin/bash  

ROOT_DIR=$(shell pwd)
OBJS_DIR=obj
AllDirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .
Sources := $(foreach n,$(AllDirs) , $(wildcard $(n)/*.c))
Objs := $(patsubst %.c,%.o, $(Sources))
Deps := $(patsubst %.c,%.d, $(Sources))
StaticLib := 
DynamicLib := 
Bin := safe
BIN_DIR=./bin/$(Bin)
#AllLibs : $(DynamicLib)  
#AllLibs : $(StaticLib)   
AllLibs : $(BIN_DIR)
  
CC = arm-none-linux-gnueabi-gcc 
# CC = arm-i2som-linux-gnueabi-gcc

CXXFLAGS = -g  -Wall  
CPPFLAGS = $(foreach n,$(AllDirs) , -I$(n))
LDFLAGS = 
MYLIB=
# SYSLIB=-lpthread 
SYSLIB=-lpthread -lm --static 

DBGCFLAGS =  -O0 -g -DDEBUG

# $(StaticLib) : $(Objs)  
#     ar rcs $@ $^  
  
# $(DynamicLib) : $(Objs)  
#     $(CC) -shared -o $@ $^ $(LDFLAGS)  

Debug:$(BIN_DIR)

$(BIN_DIR) : $(Objs)  
	$(CC) $(Objs) -o $@ $(SYSLIB) $(DBGCFLAGS) 


%.d : %.c  
	$(CC) -MT"$(<:.c=.o) $@" -MM $(CXXFLAGS) $(CPPFLAGS) $< > $@

sinclude $(Deps)

.PHONY : clean Debug install
install:
	cp ./bin/safe ../imx-rootfs/home/root/safe

clean:
	rm -f $(Objs) $(Deps) $(StaticLib) $(DynamicLib) $(BIN_DIR)


