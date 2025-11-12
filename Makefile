# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -Isrc/exposition/include -Isrc/business/include -Isrc/infrastructure/include -Isrc/include

# Executable name
EXE = payment_system.exe

# All .c files
SRCS := src/exposition/application/main.c \
		src/exposition/application/display.c \
		src/exposition/application/office.c \
        src/business/service/clinic_services.c \
        src/infrastructure/adapter/infrastructure.c

# Corresponding .o files (same folders)
OBJS := $(SRCS:.c=.o)

# Default rule
all: $(EXE)

# Link
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "Build successful: $@"

# Compile each .c into its .o
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo "Compiled: $<"

# Clean object files
clean:
	@for %%f in ($(subst /,\, $(OBJS))) do if exist %%f del /q %%f
	@echo "Object files cleaned"

# Clean executable too
fclean: clean
	@if exist $(subst /,\, $(EXE)) del /q $(subst /,\, $(EXE))
	@echo "Executable removed"

# Rebuild
re: fclean all

.PHONY: all clean fclean re
