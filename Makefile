SRCDIR = src
TGT_PREFIX = lib
TGT_NAME = awscsdk
UNAME_S := $(shell uname -s)
CXXFLAGS+=-g -std=c++11 -Iinclude -I/usr/local/Iinclude
LDFLAGS+=-L$(INSTALL_PREFIX)/lib
BUILDDIR=build
$(shell mkdir -p $(BUILDDIR) >/dev/null)

SOURCES = $(wildcard src/*.cpp)

OBJ = $(subst /src,,$(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES)))

ifeq ($(UNAME_S),Linux)
        SO_SUFFIX=so
        CFLAGS:= $(CFLAGS) -fPIC
        LDFLAGS:= $(LDFLAGS) -shared 
        INSTALL_PREFIX=/usr
endif
ifeq ($(UNAME_S),Darwin)
        SO_SUFFIX=dylib
        LDFLAGS:= $(LDFLAGS) -dynamiclib
        INSTALL_PREFIX=/usr/local
endif

AWS_LIBS = $(wildcard $(INSTALL_PREFIX)/lib/libaws-cpp-sdk-*.$(SO_SUFFIX))
AWS_LIBS := $(subst /usr/local/lib/lib,-l,$(AWS_LIBS))
AWS_LIBS := $(subst .$(SO_SUFFIX),,$(AWS_LIBS))
LDFLAGS := $(LDFLAGS) $(AWS_LIBS)

TARGET_LIB=$(TGT_PREFIX)$(TGT_NAME).$(SO_SUFFIX)
TARGET = $(BUILDDIR)/$(TARGET_LIB)

$(TARGET): $(OBJ)
		@echo [LINK] $(TARGET_LIB)
		@$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(CFLAGS) $(CXXFLAGS)

install: $(TARGET)
		@echo [INSTALL] 
		@mkdir -p $(INSTALL_PREFIX)/include/$(TGT_NAME)
		@cp include/*.h $(INSTALL_PREFIX)/include/$(TGT_NAME)/
		@cp $(TARGET) $(INSTALL_PREFIX)/lib

uninstall:
		@echo [UNINSTALL]
		@rm -rf $(INSTALL_PREFIX)/include/$(TGT_NAME)
		@rm -f $(INSTALL_PREFIX)/lib/$(TARGET_LIB)

clean:
		@echo [CLEAN]
		@rm -f $(OBJ)

.PHONY: clean

$(OBJ): $(BUILDDIR)/%.o : src/%.cpp
		@echo [C++] $<
		$(COMPILE.cpp) $(CFLAGS) $(OUTPUT_OPTION) $< 
