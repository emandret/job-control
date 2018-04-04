##########
# TARGET #
##########

NAME=jobcontrol.a
MKNAME=jobcontrol

############
# COMPILER #
############

CC=clang
CFLAGS=-Wall -Werror -Wextra

#######
# LIB #
#######

LIB = ar rc

##########
# HEADER #
##########

INC_DIR = inc/
INC_NAME = \
	data_struct.h\
	job_control.h\

INCLUDE = $(addprefix $(INC_DIR), $(INC_NAME))
INC = -I $(INC_DIR)

#######
# SRC #
#######

SRC_MAIN_DIR=\
	main/
SRC_MAIN_NAME =\
	main.c\
	main2.c\

SRC_MAIN = $(addprefix $(SRC_MAIN_DIR), $(SRC_MAIN_NAME))

SRC_DIR = src/

SRC_NAME=\
	job_init.c\
	job_launch.c\
	job_notify.c\
	job_schedule.c\
	job_util.c\
	process_exec.c\
	process_init.c\
	process_status.c\
	process_util.c\
	shell.c\

SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))

#######
# OBJ #
#######

OBJ_DIR= ./obj/
OBJ_NAME=$(SRC_NAME:.c=.o)
OBJ=$(addprefix $(OBJ_DIR), $(OBJ_NAME))

#######
# Dir #
#######

SRC_DIR_NAME =\
	$(SRC_CONV_DIR)\
	$(SRC_FLOAT_DIR)\
	$(SRC_BUFFER_DIR)\

OBJ_DIR_ALL = $(addprefix $(OBJ_DIR), $(SRC_DIR_NAME))

#########
# MACRO #
#########

NC		= "\\033[0m"
RED		= "\\033[31m"
GREEN	= "\\033[32m"
YELLOW	= "\\033[33m"
BLUE	= "\\033[34m"
MAJENTA	= "\\033[35m"
CYAN	= "\\033[36m"
BOLD	= "\\033[1m"
CHEK	= "✓"
OK		= "$(GREEN)$(CHEK)$(NC)"

#######
# NOT #
#######

ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
		@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
		-nrRf $(firstword $(MAKEFILE_LIST)) \
		ECHO="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
ECHO = printf "\r\e[K[%-10s:%10s%%]" "$(MKNAME)" "`expr "\`expr $C '*' 100 / $T\`"`"
endif

#########
# RULES #
#########

all : $(NAME)

$(NAME) : $(OBJ_DIR) $(OBJ_DIR_ALL) $(OBJ)
	@$(ECHO)
	@printf " doing ar rc $(BOLD)$(CYAN)$@$(NC)"
	@$(LIB) $@ $(OBJ)
	@printf ' '$(OK)
	@$(ECHO)
	@printf " doing ranlib $(BOLD)$(CYAN)$@$(NC)"
	@ranlib	$@
	@printf ' '$(OK)'\n'

$(OBJ_DIR) :
	@$(ECHO)
	@printf " creating folder $(MAJENTA)$(BOLD)$@$(NC)"
	@mkdir -p $(OBJ_DIR)
	@printf ' '$(OK)

$(OBJ_DIR_ALL) :
	@$(ECHO)
	@printf " creating folder $(MAJENTA)$(BOLD)$@$(NC)"
	@mkdir -p $@
	@printf ' '$(OK)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE) ./Makefile
	@$(ECHO)
	@printf " compile $(BOLD)$(YELLOW)$<$(NC) "
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC)
	@printf $(OK)

build_test : $(NAME) $(SRC_MAIN)
	@for w in $(SRC_MAIN); do\
		((number = number + 1));\
		echo $$w as test$$number;\
		$(CC) $(CFLAGS) -o test$$number $$w $(NAME) $(INC);\
	done


clean : $(OBJ_DIR)
	@$(ECHO)
	@printf " rm all $(BOLD)$(RED) obj file$(NC)"
	@rm -rf $(OBJ_DIR)
	@printf ' '$(OK)

norme : $(SRC) $(INCLUDE)
	@$(ECHO)
	@printf " checking\n"
	@norminette $^

fclean : clean
	@$(ECHO)
	@printf " rm $(BOLD)$(CYAN)$(NAME)$(NC)"
	@rm -f $(NAME)
	@printf ' '$(OK)

proper :
	@make -C ./ all
	@make -C ./ clean

re :
	@make -C ./ fclean
	@make -C ./ all

print-%:
	@echo '$($*)'

.PHONY: proper re norme all fclean clean build_test
endif
