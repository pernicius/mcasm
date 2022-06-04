# Some platform detection
# see: https://stackoverflow.com/questions/714100/
ifeq ($(OS), Windows_NT)
	uname_S := Windows
else
	uname_S := $(shell uname -s)
endif
#ifeq ($(uname_S), .....) #See https://stackoverflow.com/a/27776822/938111
#	target = .....
#endif


# ===== WINDOWS SPEZIFIC CONFIG ==============================================
ifeq ($(uname_S), Windows)
	DIRSEP = $(basename \)
	EXEEXT = .exe
endif


# ===== LINUX SPEZIFIC CONFIG ================================================
ifeq ($(uname_S), Linux)
	DIRSEP = /
	EXEEXT =
endif


# ===== MISC CONFIG ==========================================================


# Versioning
VERSION_MAJOR  = 0
VERSION_MINOR  = 1
VERSION_BUGFIX = 
VERSION_EXTRA  = dev

# Folders
SRCDIR = source
BINDIR = bin
BLDDIR = build

# Target
#TARGET = $(BINDIR)$(DIRSEP)mcasm$(EXEEXT)
TARGET = $(BINDIR)/mcasm$(EXEEXT)

# Some commands
MD = mkdir
RD = rd

# GCC commands
CC = g++
LD = g++


# ===== GENERATE FILE LISTS ==================================================


# Make does not offer a recursive wildcard function, so here's one:
# see: https://stackoverflow.com/questions/3774568/
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# cpp-sources
CPPFILES = $(strip $(call rwildcard,$(SRCDIR)/,*.cpp))

# obj-files
OBJFILES = $(patsubst %.cpp,%.obj,$(patsubst $(SRCDIR)/%,$(BLDDIR)/%,$(CPPFILES)))

# dep-files
DEPFILES = $(patsubst %.obj,%.d,$(OBJFILES))

# folder
DIRS = $(sort $(dir \
	$(OBJFILES) \
	$(TARGET) \
))


# ===== FLAGS ================================================================


# Debug flags
# see: https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
#DEBUG = -g

# Warning flags
# see: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
WFLAGS = -Wall -Wextra

# Optimization flags
# see: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
OPTFLAGS = -Og

# Generate dependencies
# -MP  generate pony targets (to suppress errors for missing headers)
# -MD  generate dependencies with system headers
# -MMD generate dependencies without system headers
# see: http://www.microhowto.info/howto/automatically_generate_makefile_dependencies.html
# see: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
DEPFLAGS = -MP -MMD

# C/C++ flags
CFLAGS  = $(WFLAGS) $(OPTFLAGS) -c $(DEBUG) $(DEPFLAGS)
CFLAGS += -DMAKE_HOST=\"$(MAKE_HOST)\"
CFLAGS += -std=c++11 # see: https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
# Versioning
ifdef VERSION_MAJOR
CFLAGS += -DVERSION_MAJOR=$(VERSION_MAJOR)
endif
ifdef VERSION_MINOR
CFLAGS += -DVERSION_MINOR=$(VERSION_MINOR)
endif
ifdef VERSION_BUGFIX
CFLAGS += -DVERSION_BUGFIX=$(VERSION_BUGFIX)
endif
ifdef VERSION_EXTRA
CFLAGS += -DVERSION_EXTRA=\"$(VERSION_EXTRA)\"
endif

# Linker flags
LFLAGS = $(WFLAGS) $(DEBUG)


# ===== RULES ================================================================


# default target
all : $(DIRS) $(TARGET)


# target executable
$(TARGET) : $(OBJFILES)
	$(CC) $(LFLAGS) $^ -o $@


# dependencies (if any)
-include $(DEPFILES)


# object files
# see: https://www.gnu.org/software/make/manual/make.html#Eval-Function
define CPPRULE =
$(patsubst $(SRCDIR)/%,$(BLDDIR)/%,$(1:%.cpp=%.obj)) : $(1) Makefile
	$$(CC) $$(strip $$(CFLAGS) $$< -o $$@)
endef
$(foreach cppfile,$(CPPFILES),$(eval $(call CPPRULE,$(cppfile))))


# folder
# note: ignored errors due to makes backslash behavior under windows
$(DIRS) :
	-@$(MD) $(subst /,$(DIRSEP),$@) 2> nul || :


# cleanup
# note: ignored errors due to makes backslash behavior under windows
clean :
	-@$(RD) /S /Q $(BINDIR) 2> nul || :
	-@$(RD) /S /Q $(BLDDIR) 2> nul || :
	