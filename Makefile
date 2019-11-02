TARGET=main
LIBS=-lGL -lGLU -lX11 -lglut -lpthread -lpng -lftgl -g
OPTIMIZE=-Og
LFLAGS=-Wall -Wextra -lm $(LIBS)
CPP=g++
LD=ld
BUILD_DIR=Build
SOURCE_DIR=Src
BIN_DIR=Binaries


CPP_INCLUDES = \
-IIncludes

CPP_SOURCES = \
Src/main.cpp \
Src/routines.cpp \
Src/findwayBases.cpp \
Src/math.cpp


OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))



CFLAGS=$(CPP_DEFS) $(CPP_INCLUDES) $(OPTIMIZE) -Wall -Wextra -std=gnu++11 -g


all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(CPP) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@


$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CPP) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@



directories: $(BUILD_DIR) $(BIN_DIR) $(SOURCE_DIR)


$(BUILD_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

$(SOURCE_DIR):
	mkdir $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*