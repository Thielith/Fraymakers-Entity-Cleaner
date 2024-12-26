COMPILER := g++
COMPILER_WINDOWS := x86_64-w64-mingw32-g++

LOOK_HERE_TOO := -L/usr/local/lib -I/usr/local/include
FLAGS_ERROR := -Wall -Wextra -Werror

ENTITY_DATA_LIB_INCLUDE := -lEntityDataExtractor
ENTITY_DATA_LIB_INCLUDE_WINDOWS := -lEntityDataExtractorWindows
INCLUDE_JSON := -ljsoncpp
INCLUDE_JSON_WINDOWS := -ljsoncpp_static

SOURCE := main.cpp creator.cpp
RESULT := EntityCleaner
RESULT_WINDOWS := $(RESULT).exe
OBJS = $(SOURCE:.cpp=.o)
DEPENDACIES := $(SOURCE:.cpp=.d)

# execute rules even if files exist
.PHONY: all clean

all: linking

linking: $(OBJS)
	$(COMPILER) -static $(FLAGS_ERROR) $(LOOK_HERE_TOO) $^ $(ENTITY_DATA_LIB_INCLUDE) $(INCLUDE_JSON) -o $(RESULT)

%.o: %.cpp
	$(COMPILER) -static $(FLAGS_ERROR) $(LOOK_HERE_TOO) -MMD -MP -c $< -o $@

windows:
	$(MAKE) COMPILER=$(COMPILER_WINDOWS) INCLUDE_JSON=$(INCLUDE_JSON_WINDOWS) ENTITY_DATA_LIB_INCLUDE=$(ENTITY_DATA_LIB_INCLUDE_WINDOWS) RESULT=$(RESULT_WINDOWS) all

clean:
	$(RM) $(OBJS) $(DEPENDACIES)
