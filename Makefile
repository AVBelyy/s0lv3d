CXX=g++
CFLAGS=
PROJECT=s0lv3d
SOURCE=src
BUILD=build
SOURCES=$(notdir $(wildcard $(SOURCE)/*.cpp))
OBJECTS=$(SOURCES:.cpp=.o)

object_files=$(addprefix $(BUILD)/,$(OBJECTS))

all: preparation $(BUILD)/$(PROJECT)

preparation: $(BUILD)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/$(PROJECT): $(object_files)
	$(CXX) $(CFLAGS) $(object_files) -o $(BUILD)/$(PROJECT)

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
