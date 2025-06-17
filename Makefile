#Colors
RED 	= \033[0;91m
GREEN	= \033[1;92m
YELLOW	= \033[1;93m
BLUE	= \033[0;96m
ORANGE	= \033[0;33m
GRAY	= \033[0;90m

# Executable name
NAME = philo

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -I$(INC_DIR)
# For debugging with Valgrind or sanitizers:
# CFLAGS += -g
# For testing with sanitizers (example: address sanitizer)
# CFLAGS += -fsanitize=address

# Source files - All paths now include SRC_DIR
SRCS =	$(SRC_DIR)/main.c \
		$(SRC_DIR)/utils.c \
		$(SRC_DIR)/init.c \
		$(SRC_DIR)/actions.c \
		$(SRC_DIR)/monitoring.c \
		$(SRC_DIR)/routine.c \
		$(SRC_DIR)/init_forks.c \
		$(SRC_DIR)/init_utility_mutexes.c \
		$(SRC_DIR)/init_core.c \
		$(SRC_DIR)/cleanup_utils.c \
		$(SRC_DIR)/thread_management.c

# Object files - Paths now include OBJ_DIR and are derived from SRCS in SRC_DIR
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(NAME)

# Linking rule - Takes objects from OBJ_DIR
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(BLUE) $(NAME_PROJECT) --> Created & compiled 👀$(END)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@test -d $(OBJ_DIR) || mkdir $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean rule - Removes the OBJ_DIR contents and then the directory
clean:
	@echo "$(GREEN) All objects files deleted 💀💀 $(END)"
	@rm -rf $(OBJ_DIR)

# Full clean rule - Calls clean and then removes executable
fclean: clean
	@echo "$(RED) $(NAME) deleted 💀💀 $(END)"
	@rm -f $(NAME)

# Rebuild rule
re: fclean all

# Phony targets
.PHONY: all clean fclean re
