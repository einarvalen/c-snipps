MORE_INCLUDES :=
MORE_DEPENDENT_OBJECT_FILES :=
MORE_DEPENDENT_LIBRARIES :=
TARGET := spike
MORE_COMPILE_OPTIONS :=
MORE_LINK_OPTIONS :=


.PHONY: test
test : all qa
	(cd test; make test)	

.PHONY: clean
clean : 
	$(RM) $(MY_OBJECT_FILES) $(D_FILES) $(TARGET)
	(cd test; make clean)	
	
