# ===============================
# Force Windows shell
# ===============================
SHELL := cmd.exe
.SHELLFLAGS := /C

# ===============================
# Compiler & flags
# ===============================
CC = gcc
CFLAGS = -Wall -Wextra -g

CPPFLAGS = -Isrc/exposition/include \
           -Isrc/business/include \
           -Isrc/infrastructure/include \
           -Isrc/include \
           -Isrc/business/port \
           -Isrc/business/model \
		   -Isrc/test/include

# ===============================
# Target
# ===============================
EXE = payment_system
OBJDIR = obj

# ===============================
# Source files (FORWARD SLASHES!)
# ===============================
SRCS = \
src/exposition/application/main.c \
src/exposition/application/app_launcher.c \
src/exposition/application/display.c \
src/exposition/application/chomp.c \
src/exposition/controller/clinic_controller.c \
src/exposition/controller/patient_controller.c \
src/exposition/adapter/clinic_process_adapter.c \
src/exposition/adapter/patient_process_adapter.c \
src/business/service/clinic_services.c \
src/business/service/patient_services.c \
src/infrastructure/adapter/clinic_infrastructure_adapter.c \
src/infrastructure/adapter/patient_infrastructure_adapter.c \
src/infrastructure/repository/service_provided_repository.c \
src/infrastructure/repository/payment_method_repository.c \
src/infrastructure/repository/patient_repository.c \
src/infrastructure/repository/department_repository.c	\
src/test/clinic_services_test.c

# ===============================
# Object files (mirrored structure)
# ===============================
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

# ===============================
# Rules
# ===============================
all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo Build successful
# cmd.exe command to create obj directory if it doesn't exist
$(OBJDIR)/%.o: src/%.c
	
	if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo Compiled $<

clean:
	if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	@echo Object files removed

fclean: clean
	if exist $(EXE).exe del /Q $(EXE).exe
	if exist $(EXE) del /Q $(EXE)
	@echo Executable removed

re: fclean all

.PHONY: all clean fclean re
