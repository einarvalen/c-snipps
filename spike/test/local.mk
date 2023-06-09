INCLUDE_DIR_FOR_TEST := ..
OBJECT_FILES_FOR_TEST := $(shell echo $(INCLUDE_DIR_FOR_TEST)/[ABCDEFGHIKJLMNOPQRSTUVWXYZ]*.o) 

MORE_INCLUDES := -I$(INCLUDE_DIR_FOR_TEST)
MORE_DEPENDENT_LIBRARIES := -lgtest
MORE_DEPENDENT_OBJECT_FILES := $(OBJECT_FILES_FOR_TEST)
TARGET := test_runner
MORE_COMPILE_OPTIONS := 
MORE_LINK_OPTIONS :=

.PHONY: test
test : all qa
	./$(TARGET)

.PHONY: clean
clean : 
	$(RM) $(MY_OBJECT_FILES) $(D_FILES) $(TARGET)

