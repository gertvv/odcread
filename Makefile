# Modules, each containing:
#  * $(MODULE)/Make.inc -- makefile include
#  * $(MODULE)/$(MODULE).h -- minimal "interface" header file
#  * $(MODULE)/$(MODULE).ih -- internal/implementation header file
#    (only to be included by sources within the module)
#  * $(MODULE)/*.cc -- module source files
MODULES := main reader store alien typeregister textmodel fold typepath

CFLAGS += -I.

# Variables for the modules to write to
SRCS := 

# Include module definitions
include $(patsubst %,%/Make.inc,$(MODULES))

# This rule just links the object files together
odcread: $(SRCS:.cc=.o)
	g++ -o $@ $^

# This rule build an object (.o) from a source (.cc). 
%.o: %.cc
	g++ $(CFLAGS) $< -c -o $@

# For each implementation header (.ih) generate a dependency (.d) file which
# will ensure that the compiled header (.ih.gch) is rebuilt whenever the header
# files included by the .ih are updated.
# The options given to GCC for this are as follows:
#  -MM : calculate dependencies, but exclude system headers
#  -MF : output dependencies to the given file
#  -MP : generate "header.h:" rules to avoid errors on deletion of headers
#  -MT : the main rule has the given target (to handle subdirs correctly)
# The .d file is not an explicit target because it will need to be (re-)built
# if and only if the .ih.gch needs to be rebuilt.
%.ih.gch: %.ih
	g++ $(CFLAGS) -x c++-header $< -MM -MF $*.d -MP -MT $@
	g++ $(CFLAGS) -x c++-header $< -o $@

# Each module has a .ih file that should be *the only* include from the .cc
# files. These .ih files are pre-compiled to .ih.gch, and dependency caching
# is based on the .ih files, not the .cc files.
define depend_on_compiled_header
$(patsubst %.cc,%.o,$(1)) : $(dir $(1))module.ih.gch
endef
$(foreach src,$(SRCS),$(eval $(call depend_on_compiled_header,$(src))))

clean:
	rm -f odcread *.o */*.o */*.d */*.ih.gch

# Include the generated dependency files (if they exist)
-include $(patsubst %,%/module.d,$(MODULES))
