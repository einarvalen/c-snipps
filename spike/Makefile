SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .h .cpp .o
CPP := g++
RM := rm -f

include local.mk

INCLUDES := $(MORE_INCLUDES) 
CPPFLAGS := $(INCLUDES) -O0 -g3 -std=c++11 -Wall -Wextra -Werror -m64 -fmessage-length=0 -MMD -MP $(MORE_COMPILE_OPTIONS)
DEPENDENT_LIBRARIES := -pthread -lrt -ldl $(MORE_DEPENDENT_LIBRARIES)
DEPENDENT_OBJECT_FILES = $(MORE_DEPENDENT_OBJECT_FILES)
LINK_OPTIONS := -m64 $(MORE_LINK_OPTIONS)
HEADER_FILES := $(shell echo *.h)
SOURCE_FILES := $(shell echo *.cpp)
MY_OBJECT_FILES := $(subst .cpp,.o, $(SOURCE_FILES) ) 
OBJECT_FILES := $(MY_OBJECT_FILES) $(DEPENDENT_OBJECT_FILES)
D_FILES := $(subst .cpp,.d, $(SOURCE_FILES) )

.PHONY: qa
qa :
	cppcheck --enable=style,performance,portability,unusedFunction --platform=unix64 --std=c++03 -f $(INCLUDES) -q *.cpp

.PHONY: all
all : $(TARGET)

$(TARGET) : $(OBJECT_FILES) 
	$(CPP) $(LINK_OPTIONS) -o $@ $(OBJECT_FILES)  $(DEPENDENT_LIBRARIES)
	
%.o : %.c $(HEADER_FILES)
	$(CPP) $(CPPFLAGS) -c $@ $<

	
