# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoel <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 16:36:11 by yoel              #+#    #+#              #
#    Updated: 2023/07/18 19:32:44 by ycornamu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors

GREEN	= \033[1;32m
RED		= \033[1;31m
ORANGE	= \033[1;33m
BUILD	= \e[38;5;225m
SEP		= \e[38;5;120m
DUCK	= \e[38;5;227m
RESET	= \033[0m

# Colors

NAME		= ft_irc

SRCS 		= main.cpp \
			  IRCServer.cpp \
			  Client.cpp \
			  Channel.cpp \
			  Message.cpp \
			  commands/Cap.cpp \
			  commands/Nick.cpp \
			  commands/User.cpp \
			  commands/Ping.cpp \
			  commands/Pong.cpp \
			  commands/Quit.cpp \
			  commands/Pass.cpp \
			  commands/Privmsg.cpp

SRCS_DIR	= srcs

OBJS		= $(SRCS:.cpp=.o)
OBJS_DIR 	= objs

_SRCS 		= $(addprefix $(SRCS_DIR)/, $(SRCS))
_OBJS 		= $(addprefix $(OBJS_DIR)/, $(OBJS))

DIRS 		= $(OBJS_DIR) $(OBJS_DIR)/commands

HEADERS 	= includes

CC 			= c++
CFLAGS 		= -I $(HEADERS) -Wall -Wextra -Werror -std=c++98
LFLAGS 		=

DEBUG_CFLAGS = -g -fsanitize=address -fno-omit-frame-pointer
DEBUG_LFLAGS = -fsanitize=address


all: $(NAME)

leak: LFLAGS += $(DEBUG_LFLAGS)
leak: CFLAGS += $(DEBUG_CFLAGS)
leak: $(NAME)

$(NAME): $(_OBJS)
	@printf "\n"
	@$(CC) $(_OBJS) $(LFLAGS) -o $(NAME)
	@printf "${GREEN} 💻 Successfully created ${NAME}${RESET} ✅\n"

$(_OBJS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp $(DIRS)
	@${CC} ${CFLAGS} -o $@ -c $< -I${HEADERS}
	@printf "\e[1K\r${BUILD} 🚧 $@ from $<${RESET}"

$(DIRS):
	@mkdir -p $(DIRS)

clean:
	@rm -rf $(DIRS)
	@printf "${RED}Deleted ${NAME} objects${RESET}\n"

fclean: clean
	@rm -rf $(NAME)
	@printf "${RED}Deleted ${NAME} bin${RESET}\n"

re: fclean all

.PHONY : all clean fclean re
