# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target Executable Name
TARGET = projeto

# Source Files
SRCS = projeto.c parser.c commands.c utils.c connections.c estruturas.c

# Object Files (automatically derived from source files)
OBJS = $(SRCS:.c=.o)

# Default Target
all: $(TARGET)

# Link Object Files to Create the Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic Rule to Compile .c Files into .o Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up Target
clean:
	rm -f $(OBJS) $(TARGET)

# Phony Targets
.PHONY: all clean