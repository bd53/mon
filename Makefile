c = gcc
cflags = -Wall -Wextra -O2 -Iinclude
ldflags = -lncurses -lcurl
target = build

.PHONY: all build run clean

all: clean build

build:
	@echo "Finding C source files..."
	@for src in $$(find . -name '*.c'); do \
		obj="$${src%.c}.o"; \
		dir=$$(dirname "$$obj"); \
		mkdir -p "$$dir"; \
		echo "  ✓ $$src"; \
		$(c) $(cflags) -c "$$src" -o "$$obj"; \
	done
	@echo "Linking object files..."
	@$(c) $(cflags) -o $(target) $$(find . -name '*.o') $(ldflags)
	@echo "Build complete: $(target)"

run:
	@echo "Running $(target)..."
	@./$(target)

clean:
	@echo "Cleaning build artifacts..."
	@rm -f $(target)
	@find . -name '*.o' -type f -delete
	@echo "Clean complete"