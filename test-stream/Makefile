VERBOSE = @
OBJDIR = build
CXX = g++
MKDIR = mkdir
CC_SOURCES = ../test-stream/console_out.cc ../test-stream/test.cc ../test-stream/file_out.cc ../object/o_stream.cc ../object/strbuf.cc ../test-stream/console_out.cc ../test-stream/test.cc ../test-stream/file_out.cc ../object/o_stream.cc ../object/strbuf.cc
CXXFLAGS = -std=c++11 -m32 -I../object -I.
TARGET = test

all: $(TARGET)

$(TARGET): $(CC_SOURCES)
	@echo "CXX		$@"
	$(VERBOSE) $(MKDIR) -p $(OBJDIR)
	$(VERBOSE) $(CXX) -o $(OBJDIR)/$@ $(CXXFLAGS) $^

clean:
	@echo "RM		$(OBJDIR)"
	$(VERBOSE) rm -rf $(OBJDIR)

.PHONY: all clean
