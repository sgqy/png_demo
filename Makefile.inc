CC = clang++
LD = clang++

SOURCE = $(wildcard ./*.cpp)
HEADER = $(wildcard ./*.hpp)
OBJECT = $(patsubst ./%.cpp, obj/%.o, $(SOURCE))

PCH_SRC = stdafx.hpp
PCH_BIN = stdafx.hpp.pch 

all: $(TARGET)

run: $(TARGET)
	$(PWD)/$(TARGET)

$(TARGET): $(OBJECT)
	$(LD) -o $@ $^ $(LDFLAGS)

obj/%.o: %.cpp $(HEADER) $(PCH_BIN)
	$(CC) $(CFLAGS) -include $(PCH_SRC) -c -o $@ $<

$(PCH_BIN): $(PCH_SRC)
	-mkdir obj 2>/dev/null
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET) $(PCH_BIN) obj/

.PHONY: all run clean
