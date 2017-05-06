# Directories
SRCDIR = src
OBJDIR = bld

# Compilers
CC ?= gcc
CXX ?= g++

# Shared compilation flags
CFLAGS = -ansi -pedantic -Wall -Wextra -Wunreachable-code
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces -Winline
CFLAGS+= -Wno-missing-field-initializers

BUILD ?= release
ifeq ($(BUILD), debug)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O3
endif

# Specific flags
CXFLAGS = $(CFLAGS)

# Executables
EXEC_PROJ = $(OBJDIR)/proj
EXEC_PROJPP = $(OBJDIR)/projpp
EXECS = $(EXEC_PROJ) $(EXEC_PROJPP)

# General rules (point to main focus files)
all: proj

clean:
	rm -rf $(EXECS) $(OBJDIR)/*.o $(OBJDIR)/*.dSYM

# Specific rules
proj: $(EXEC_PROJPP)

# Compilation rules
$(EXEC_PROJ): $(SRCDIR)/proj.c
	$(CC) $(CFLAGS) $^ -o $@

$(EXEC_PROJPP): $(SRCDIR)/proj.cpp
	$(CXX) $(CXFLAGS) $^ -o $@

.PHONY: all clean
