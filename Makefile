# Makefile for this project
# TODO: need to make make file more system agnostic e.g not assum taht people have librarys installed in usr/local/include 
# TODO: eigen has an issue with compilation, so I had to remove the -Werror falg from compiler options. I should not need to do this.
srcPrefix = src/
buildPrefix = build/
testPrefix = test/


srcFiles := $(shell find $(srcPrefix) -maxdepth 1 -name '*.cpp')
testFiles := $(shell find $(srcPrefix)$(testPrefix) -name '*.cpp')
testFiles := $(foreach file,$(testFiles),$(patsubst $(srcPrefix)$(testPrefix)%.cpp, %, $(file)))

objectFiles := $(foreach file,$(srcFiles),$(patsubst $(srcPrefix)%.cpp,$(buildPrefix)%.o,$(file)))
dependencies := $(foreach file,$(srcFiles),$(patsubst $(srcPrefix)%.cpp,$(buildPrefix)%.d,$(file)))

compilerOptions = -g3 -std=c++17 -I ./hdr -I ./thirdParty -I /usr/include/eigen3 -Wall  -lopenpose -lgflags 

# includes other make file rules
-include $(dependencies)

# Tells make which relative directory to search for the specifc file types in
vpath %.o build
vpath %.d build
vpath %.cpp src
vpath %.hpp hdr

$(objectFiles): %.o : %.d
	g++ -c -o $@ $(compilerOptions) $(patsubst $(buildPrefix)%.o, $(srcPrefix)%.cpp, $@)

# -MM flag spits out all of the non system dependencies, may need to change to M to get all of the system dependencies
$(dependencies): $(buildPrefix)%.d : %.cpp
	echo $^
	g++ -MM $^ $(compilerOptions) |\
	sed 's/$(patsubst $(srcPrefix)%.cpp,%.o,$^)/$(patsubst $(srcPrefix)%.cpp,build\/%.o,$^) $(patsubst $(srcPrefix)%.cpp,build\/%.d,$^)/g' > $@

# export LD_LIBRARY_PATH is needed to make sure that the loader finds openpose
# $$ becomes $ in make
# $@ is the target, $^ is a list of all of the dependencies
# recompile if the object files change or any of the headers change
$(buildPrefix)FastDash: $(objectFiles)
	export LD_LIBRARY_PATH="LD_LIBRARY_PATH:/usr/local/lib"; \
	g++ -o $@ $^ $(compilerOptions)



.PHONY: clean
.PHONY: all
.PHONY: FastDash
.PHONY: print

FastDash : $(buildPrefix)FastDash

$(testFiles) : % : $(buildPrefix)test/%

$(buildPrefix)test/% : $(objectFiles) $(srcPrefix)/test/%.cpp
	export LD_LIBRARY_PATH="LD_LIBRARY_PATH:/usr/local/lib"; \
	g++ -g3 -o $@  $(patsubst $(buildPrefix)main.o, ,$^) $(compilerOptions)


print:
	echo $(dependencies)

all: $(objectFiles) $(buildPrefix)FastDash

clean:
	rm -r ./build/*
	mkdir ./build/test