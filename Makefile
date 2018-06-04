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
  
CC = gcc  
CXXFLAGS = -g  -Wall  
CPPFLAGS = $(foreach n,$(AllDirs) , -I$(n))
LDFLAGS = 
MYLIB=
SYSLIB=-lpthread
# $(StaticLib) : $(Objs)  
#     ar rcs $@ $^  
  
# $(DynamicLib) : $(Objs)  
#     $(CC) -shared -o $@ $^ $(LDFLAGS)  
  
$(BIN_DIR) : $(Objs)  
	$(CC) $(Objs) -o $@ $(SYSLIB)

%.d : %.c  
	$(CC) -MT"$(<:.c=.o) $@" -MM $(CXXFLAGS) $(CPPFLAGS) $< > $(ROOT_DIR)/$(OBJS_DIR)/$@

sinclude $(Deps)

.PHONY : clean
clean:
	rm -f $(Objs) $(Deps) $(StaticLib) $(DynamicLib) $(BIN_DIR)