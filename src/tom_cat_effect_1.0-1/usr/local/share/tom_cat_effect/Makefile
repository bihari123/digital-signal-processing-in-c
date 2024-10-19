CC = gcc
CFLAGS = -I./include -Wall -Wextra -pedantic
LIBS = -lportaudio -lm

TARGET = tom_cat_effect
SRC = src/tom_cat_effect.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
