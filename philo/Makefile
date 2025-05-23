NAME := philo
OBJECT_DIR := obj
INCLUDE_DIR := includes
SRC_DIR := srcs
FILES := main.c \
		time_utils.c \
		print_utils.c \
		check_utils.c \
		fork_utils.c \
		eat_utils.c \
		thread_routines.c \
		init_utils.c \
		philo_init_utils.c \
		simulation_utils.c \
		cleanup_utils.c \
		monitor_utils.c \
		parse_utils.c
SRCS := $(addprefix $(SRC_DIR)/, $(FILES))
CFLAGS := -Wall -Wextra -Werror -pthread
# DEBUG=1 make re to compile with debug flags
ifdef DEBUG
    CFLAGS += -DDEBUG=1 -g3
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=thread
endif
IFLAGS := -I$(INCLUDE_DIR)
LFLAGS := -lpthread
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
CC := gcc
DATA_RACE := valgrind --tool=drd
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

.PHONY: re
re: fclean all

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: leak
leak: $(NAME)
	$(LEAK_TOOL) ./$(NAME) $(ARGS)

.PHONY: data_race
data_race: $(NAME)
	$(DATA_RACE) ./$(NAME) $(ARGS)
