# Makefile used to build mex files

TARGETS = all clean
.PHONY: $(TARGETS)

ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif

ifeq ($(OSTYPE),linux)
  MEXSUFFIX = mexglx
endif
ifeq ($(OSTYPE),darwin)
  MEXSUFFIX = mexmaci64
endif

MATMEX=mex
MEX = $(MATMEX)
CXXFLAGS = -O

all: yuyv2label.$(MEXSUFFIX) \
	block_bitor.$(MEXSUFFIX) \
	color_count.$(MEXSUFFIX) \
	color_stats.$(MEXSUFFIX) \
	connected_regions.$(MEXSUFFIX) \
	goal_posts.$(MEXSUFFIX) \
	field_lines.$(MEXSUFFIX) \
	field_occupancy.$(MEXSUFFIX) \
	field_spots.$(MEXSUFFIX)\
	label_to_mask.$(MEXSUFFIX)

15a: MATMEX=/Applications/MATLAB_R2015a.app/bin/mex
15a: all

14a: MATMEX=/Applications/MATLAB_R2014a.app/bin/mex
14a: all

14b: MATMEX=/Applications/MATLAB_R2014b.app/bin/mex
14b: all

13a: MATMEX=/Applications/MATLAB_R2013a.app/bin/mex
13a: all

13b: MATMEX=/Applications/MATLAB_R2013b.app/bin/mex
13b: all

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -fPIC -o $@ -c $<
%.o: %.cc
	$(MEX) $(CXXFLAGS) $(INCLUDE_DIRS) -fPIC -o $@ -c $<
%.o: %.c
	$(CC) $(CXXFLAGS) $(INCLUDE_DIRS) -fPIC -o $@ -c $<

%.$(MEXSUFFIX): %.cc
	$(MEX) $(CXXFLAGS) $<

connected_regions.$(MEXSUFFIX): connected_regions.cc RegionProps.o ConnectRegions.o
	$(MEX) $(CXXFLAGS) $^

goal_posts.$(MEXSUFFIX): goal_posts.cc RegionProps.o
	$(MEX) $(CXXFLAGS) $^

goal_crossbar.$(MEXSUFFIX): goal_crossbar.cc RegionProps.o
	$(MEX) $(CXXFLAGS) $^

field_lines.$(MEXSUFFIX): field_lines.cc RadonTransform.o
	$(MEX) $(CXXFLAGS) $^

field_spots.$(MEXSUFFIX): field_spots.cc RegionProps.o ConnectRegions.o
	$(MEX) $(CXXFLAGS) $^

clean:
	rm -f *.$(MEXSUFFIX) *.o
