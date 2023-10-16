CXX   	= g++
CCFLAGS = -std=c++17 -Wall -Wextra -Werror

# Build directory must be created before running make.
BUILDDIR = build

# Put test executables and main executable here
EXEC = parse

# Put dependent object builds here.
OBJS = 	$(BUILDDIR)/Lexer.o	\
		$(BUILDDIR)/AST.o \
		$(BUILDDIR)/Nodes.o \
		$(BUILDDIR)/Token.o \

.PHONY: clean

all: $(EXEC) $(OBJS)

%: src/%.cpp $(OBJS)
	${CXX} $(CCFLAGS) -o $(BUILDDIR)/$@ $^

$(BUILDDIR)/%.o: src/lib/%.cpp
	$(CXX) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(BUILDDIR)/*

