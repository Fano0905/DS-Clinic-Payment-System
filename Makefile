# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Isrc/exposition/include -Isrc/business/include -Isrc/infrastructure/include -Isrc/include -Isrc/business/port

# Executable name
EXE = payment_system.exe

# All .c files
SRCS := src/exposition/application/main.c \
		src/exposition/application/app_launcher.c \
		src/exposition/application/display.c \
		src/exposition/application/office.c \
		src/exposition/application/patient.c \
		src/exposition/application/chomp.c \
		src/exposition/adapter/clinic_process_adapter.c \
		src/exposition/adapter/patient_process_adapter.c	\
        src/business/service/clinic_services.c \
		src/business/service/patient_services.c \
		src/infrastructure/adapter/clinic_adapter.c	\
		src/infrastructure/adapter/patient_adapter.c

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
