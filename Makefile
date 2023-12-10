# Author: Josef Kuchař (xkucha28)
CPPFLAGS = -std=c++20 -O2 -Wall -Wextra -g

# Get all .c files
SRCS = $(wildcard src/*.cc)
# Get corresponding .o files
OBJS := $(SRCS:src/%.cc=obj/%.o)
# Get corresponding .d files
DEPS := $(SRCS:src/%.cc=obj/%.d)

# These will run every time (not just when the files are newer)
.PHONY: run clean doc

all: obj model

model: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Dependecies
obj/%.o: src/%.cc obj/%.d
	$(CC) -MT $@ -MMD -MP -MF obj/$*.d $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<
$(DEPS):
include $(wildcard $(DEPS))

obj:
	mkdir -p obj

clean:
	rm -rf obj model *.zip

run: all
	./model

doc:
	pandoc -M reference-section-title=Literatura --csl style.csl --bibliography=doc/doc.bib --filter pandoc-citeproc -V lang=cs -V linkcolor=blue -V urlcolor=blue -V block-headings -N doc/doc.md -o doc.pdf

experiment-diameter: all
	rm -rf diameter.csv
	./script/diameter.sh

experiment-approach-count: all
	rm -rf approach-count.csv
	./script/approach-count.sh

experiment-default: all
	rm -rf default.csv
	./script/default.sh

zip:
	zip -r 08_xkucha28_xsirov00.zip doc.pdf src script Makefile README.md
