CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lncurses
TARGET = build
SRC_MAIN = main.c
SRC_MENU = src/menu/menu.c src/menu/menu_items.c
SRC_SYSTEM = src/system/system_info.c src/system/hardware.c src/system/software.c
SRC_UI = src/ui/screen.c src/ui/display.c
SRC_UTILS = utils/command.c utils/string_utils.c
SRC = $(SRC_MAIN) $(SRC_MENU) $(SRC_SYSTEM) $(SRC_UI) $(SRC_UTILS)
OBJ = $(SRC:.c=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)
