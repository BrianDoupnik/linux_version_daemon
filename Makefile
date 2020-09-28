#
# Filename     Makefile
# Date         4/19/2020
# Author       Brian Doupnik
# Email        bcd170030
# Course       CS 3377.502 Spring 2020
# Version      1.0
# Copyright    2020, All Rights Reserved
#
#Description
#	Makefile for the dirmond

# setup needed values to support Make's implicit commands

# the compiler
CXX = g++

# preprocessor flags
CPPFLAGS = -I ~/installedPrograms/include/

# compiler flags
CXXFLAGS = -Wall 

# linker flags
LDFLAGS = -static -L/home/011/b/bc/bcd170030/installedPrograms/lib/

# linker libraries
LDLIBS = -lrudeconfig

# project name, used to build backup target
PROJECTNAME = HW5

# the names of the c++ source files to be compiled
SOURCE = hw5main.cc hw5tclap.cc hw5rude.cc hw5dirmon.cc hw5popen.cc

# the name of the executable to be created
EXEC = bcd_dirmond

# you normally don't need to change anything below here

OBJS = $(SOURCE:cc=o)

.PHONY: all clean

all: $(EXEC)
	rm -f $(OBJS) *.d *~
	etags *.cc
clean:
	rm -f $(OBJS) *.d *~
	rm -f TAGS

# Pattern for .d files.
%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


#  This is a rule to link the files.  Pretty standard
$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(LDFLAGS) $(OBJS) $(LDLIBS)

Makefile: $(SRCS:.cc=.d)


# Backup Target
backup: clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


# Try to include the dependency files created by the PreProcessor
-include $(SRCS:.cc=.d)
