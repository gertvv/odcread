MODULES := reader store alien typeregister textmodel fold typepath

# Add module directories to the include path
CFLAGS += -I.

# Variables for the modules to write to
SRCS := odcread.cc

# Include module definitions
include $(patsubst %,%/Make.inc,$(MODULES))

# This rule just links the object files together
odcread: $(SRCS:.cc=.o)
	g++ -o $@ $^

# This rule build an object (.o) from a source (.cc). It first builds a
# dependency (.d) file which will ensure that the .o is rebuilt whenever the
# header files included by the .cc are updated.
# The options given to GCC for this are as follows:
#  -MM : calculate dependencies, but exclude system headers
#  -MF : output dependencies to the given file
#  -MP : generate "header.h:" rules to avoid errors on deletion of headers
#  -MT : the main rule has the given target (to handle subdirs correctly)
# The .d file is not an explicit target because it will need to be (re-)built
# if and only if the .o needs to be rebuilt.
%.o: %.cc
	g++ $(CFLAGS) $< -MM -MF $*.d -MP -MT $@
	g++ $(CFLAGS) $< -c -o $@

clean:
	rm -f odcread *.d *.o */*.o */*.d

# Include the generated dependency files (if they exist)
-include $(SRCS:.cc=.d)
