# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/06 13:11:21 by aulopez           #+#    #+#              #
#    Updated: 2021/10/21 14:44:01 by aulopez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=ft_nm
COMPILER=gcc
FLAGS=-g3 -Werror -Wall -Wextra -MMD -MP

VPATH=src:src/elf64:src/elf32:src/fatelf:src/ar:src/mylib
PATH_OBJ=.object/
PATH_HDR=./inc/

CC_O=$(COMPILER) $(FLAGS) -c -I$(PATH_HDR)
CC_C=$(COMPILER) $(FLAGS) $(OBJ)

MAIN=main load_argv elf_identifier my_endian my_handler my_printer
ELF64=main_elf64 head_elf64 sections_elf64 symbols_elf64
ELF32=main_elf32 head_elf32 sections_elf32 symbols_elf32
FATELF=main_fat
ARCH=main_ar
MYLIB=ft_memcmp ft_memcpy ft_memset ft_strchr ft_strcmp ft_strlen ft_strncmp

SRC=$(MAIN) $(ELF64) $(ELF32) $(FATELF) $(ARCH) $(MYLIB)
OBJ=$(SRC:%=$(PATH_OBJ)%.o)
DEP=$(OBJ:%.o=%.d)

# --- Rules --------------------------------------------------------------------

all: $(NAME)

$(NAME): $(OBJ)
	-@printf " ===> \e[32mCreated executable\e[0m: $(NAME)\n"
	@$(CC_C) -o $(NAME)
	-@printf "\n\n> \e[33m\`export LC_ALL=C\`or \`export LC_ALL=POSIX\` to compare output with system nm\e[0m\n" 
	-@printf "> \e[33mBehavior of .init_array and .fini_array have been changed since version 18.04.5 LT on Ubuntu\e[0m\n" 
	-@printf "> \e[33mThis ft_nm default sorting is alphabetic > address value > symbol letter\e[0m\n" 


$(PATH_OBJ)%.o:%.c | $(PATH_OBJ)
	-@printf " > \e[33m$*.o\e[0m\n"
	@$(CC_O) $< -o $@

$(PATH_OBJ):
	-@printf " ===> \e[32mCreated directory\e[0m: $(PATH_OBJ)\n"
	@mkdir -p $@

clean:
	-@printf " ===> \e[31mDeleted directory\e[0m: $(PATH_OBJ)\n"
	@rm -rf $(PATH_OBJ)

fclean: clean
	-@printf " ===> \e[31mDeleted executable\e[0m: $(NAME)\n"
	@rm -f $(NAME)

re: fclean $(PATH_OBJ) all

.PHONY: all clean fclean re

-include $(DEP)
