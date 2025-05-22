NAME := philo
BONUS_NAME :=
OBJECT_DIR := obj
INCLUDE_DIR := includes
SRC_DIR := srcs
BONUS_DIR :=
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
BONUS_FILES :=
CFLAGS := -Wall -Wextra -Werror -g3 -pthread
# DEBUG=1 make re to compile with debug flags
ifdef DEBUG
    CFLAGS += -DDEBUG=1 -g
endif
ifdef SANITIZE
	CFLAGS += -fsanitize=thread
endif
IFLAGS := -I$(INCLUDE_DIR)
LFLAGS := -lpthread
OBJECTS := $(addprefix $(OBJECT_DIR)/,$(FILES:.c=.o))
BONUS_OBJ := $(addprefix $(BONUS_DIR)/,$(BONUS_FILES:.c=.o))
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

.PHONY: data_race
data_race: $(NAME)
	$(DATA_RACE) ./$(NAME) $(ARGS)
