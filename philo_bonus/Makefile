NAME := philo_bonus
BONUS_NAME :=
OBJECT_DIR := obj
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR :=
FILES := utils_bonus.c \
		time_utils_bonus.c \
		print_utils_bonus.c \
		philosopher_bonus.c \
		main_bonus.c \
		init_bonus.c \
		monitor_bonus.c \
		simulation_bonus.c \
		philo_actions_bonus.c \
		philo_utils_bonus.c \
		cleanup_bonus.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
BONUS_FILES :=
CFLAGS := -Wall -Wextra -Werror -g
# DEBUG=1 make re to compile with debug flags
ifdef DEBUG
    CFLAGS += -DDEBUG=1 -g
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=address
endif
IFLAGS := -I$(INCLUDE_DIR)
LFLAGS := -lpthread
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
BONUS_OBJ := $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
CC := gcc
LEAK_TOOL := valgrind --leak-check=full
ARGS ?= 5 800 200 200

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LFLAGS) -o $(NAME)

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


.PHONY: bonus
bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(NAME)
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(OBJS_WITHOUT_MAIN) $(LFLAGS) -o $(BONUS_NAME)

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -f $(BONUS_OBJ)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

.PHONY: leak
leak: $(NAME)
	$(LEAK_TOOL) ./$(NAME) $(ARGS)

