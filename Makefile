# Makefile for this project
# TODO: need to make make file more system agnostic e.g not assume that people have librarys installed in usr/local/include 
# TODO: eigen has an issue with compilation, so I had to remove the -Werror falg from compiler options. I should not need to do this.
# TODO: add bsack in the -Wall flag to compilation
srcPrefix := src
buildPrefix := build
testPrefix := test
testSrcDir = src/test/
testSrcDirEscaped = src\/test\/

testTargetDir = build/test/
testTargetDirEscaped = build\/test\/

OPT := -g3
compilerOptions = -std=c++17 $(OPT) -I ./hdr -I ./thirdParty -I /usr/include/eigen3  -lopenpose -lgflags 



srcFiles :=    $(foreach   file, $(shell find $(srcPrefix) -maxdepth 1 -name '*.cpp'), $(patsubst $(srcPrefix)/%, %, $(file)))
srcFiles := 	$(foreach   file, $(srcFiles), $(patsubst main.cpp, , $(file))  )
srcFiles := 	$(foreach   file, $(srcFiles), $(patsubst testMain.cpp, , $(file))  )
hdrFiles :=     $(foreach   file, $(srcFiles), $(patsubst %.cpp, %.hpp, $(file))  )
objectFiles  := $(foreach   file, $(srcFiles), $(patsubst %.cpp, $(buildPrefix)/%.o,$(file)) )
objectFiles  := $(foreach   file, $(objectFiles), $(patsubst $(buildPrefix)/test%.o,,$(file)) )

dependencies := $(foreach   file, $(srcFiles), $(patsubst %.cpp, $(buildPrefix)/%.d,   $(file)) )

testSrc     := $(shell find $(srcPrefix) -maxdepth 1 -name 'test*.cpp')
testDeps	:= $(foreach file, $(testSrc), $(patsubst $(srcPrefix)%.cpp, $(buildPrefix)%.d, $(file)))
testObjects := $(foreach file, $(testSrc), $(patsubst $(srcPrefix)%.cpp, $(buildPrefix)%.o, $(file)))
testObjects := $(foreach   file, $(testObjects), $(patsubst build/testMain.o, , $(file))  )


# Tells make to search in thes directories
VPATH = hdr src src/test
-include $(dependencies)
-include $(testDeps)

$(dependencies) : $(buildPrefix)/%.d : $(srcPrefix)/%.cpp
	g++ -MM $< $(compilerOptions) |\
	sed 's/$(patsubst $(buildPrefix)/%.d,%.o,$@)/$(patsubst $(buildPrefix)/%.d,$(buildPrefix)\/%.o,$@) $(patsubst $(buildPrefix)/%.d,$(buildPrefix)\/%.d,$@)/g' > $@

$(objectFiles) $(testObjects): $(buildPrefix)/%.o : $(srcPrefix)/%.cpp $(buildPrefix)/%.d
	g++ -c -o $@ $< $(compilerOptions) 

.PHONY: clean
.PHONY: all
.PHONY: FastDash
.PHONY: print
.PHONY: Test

$(buildPrefix)/FastDash : $(objectFiles) main.cpp
	export LD_LIBRARY_PATH="LD_LIBRARY_PATH:/usr/local/lib"; \
	g++ -o $@ $^ $(compilerOptions)

$(buildPrefix)/testMain : $(testObjects) $(objectFiles) testMain.cpp
	export LD_LIBRARY_PATH="LD_LIBRARY_PATH:/usr/local/lib"; \
	g++ -o $@ $^ $(compilerOptions) 

FastDash : $(buildPrefix)/FastDash

Test : build/testMain

print:
	echo $(OPT)

all: $(objectFiles) $(buildPrefix)FastDash

clean:
	rm -r ./build/*