SRCDIR = src
TGT_PREFIX = lib
TGT_NAME = awscsdk
UNAME_S := $(shell uname -s)
CFLAGS+=-g
CXXFLAGS+=-std=c++11 -Iinclude -I/usr/local/Iinclude
BUILDDIR=build
$(shell mkdir -p $(BUILDDIR) >/dev/null)

SOURCES = $(wildcard src/*.cpp)

OBJ = $(subst /src,,$(patsubst %.cpp,$(BUILDDIR)/%.o,$(SOURCES)))
INSTALL_PREFIX=/usr/local
ifeq ($(UNAME_S),Linux)
	SO_SUFFIX=so
	CFLAGS:= $(CFLAGS) -fPIC
	LIB_LDFLAGS=-shared 
endif
ifeq ($(UNAME_S),Darwin)
	SO_SUFFIX=dylib
	LIB_LDFLAGS=-dynamiclib
endif

LDFLAGS+=-L$(INSTALL_PREFIX)/lib

AWS_LIBS = $(wildcard $(INSTALL_PREFIX)/lib/libaws-cpp-sdk-*.$(SO_SUFFIX))
AWS_LIBS := $(subst /usr/local/lib/lib,-l,$(AWS_LIBS))
AWS_LIBS := $(subst .$(SO_SUFFIX),,$(AWS_LIBS))
LDFLAGS := $(LDFLAGS) $(AWS_LIBS)

TARGET_LIB=$(TGT_PREFIX)$(TGT_NAME).$(SO_SUFFIX)
TARGET = $(BUILDDIR)/$(TARGET_LIB)

$(TARGET): $(OBJ)
	@echo [LINK] $(TARGET_LIB)
	@$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) $(LIB_LDFLAGS)

check_awssdk:
	@echo [CHECK AWS_SDK]
	@$(CXX) check_awssdk.cpp $(LDFLAGS) $(CFLAGS) $(CXXFLAGS) -o $(BUILDDIR)/$@

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

.PHONY: clean check_awssdk

$(OBJ): $(BUILDDIR)/%.o : src/%.cpp
	@echo [C++] $<
	@$(COMPILE.cpp) $(CFLAGS) $(OUTPUT_OPTION) $< 
