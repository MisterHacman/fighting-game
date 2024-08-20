TARGET := main
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src%,obj%, $(patsubst %.c,%.o,$(SOURCES)))

INCLUDE := -Iinclude -I/usr/include/SDL2
LIBPATH :=
LIBS := -lSDL2 -lSDL2_image

FLAGS := -Wall -g
CCFLAGS := $(FLAGS) -std=c11

CC := gcc

all: $(OBJECTS)
	$(CC) $(CCFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBPATH) $(LIBS)

obj/%.o: src/%.c include/%.h
	if [ ! -d "obj" ]; then mkdir obj; fi
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: clean

clean:
	rm -rf obj/
	rm -f $(TARGET)
