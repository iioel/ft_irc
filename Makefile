# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoel <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 16:36:11 by yoel              #+#    #+#              #
#    Updated: 2023/07/13 14:34:46 by ycornamu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

SRCS = main.cpp
SRCS_DIR = srcs

OBJS = $(SRCS:.cpp=.o)
OBJS_DIR = objs

_SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS))
_OBJS = $(addprefix $(OBJS_DIR)/, $(OBJS))

DIRS = $(OBJS_DIR)

HEADERS = includes

CC = c++
CFLAGS = -I $(HEADERS) -Wall -Wextra -Werror -std=c++98
LFLAGS =

DEBUG_CFLAGS = -g -fsanitize=leak -fno-omit-frame-pointer
DEBUG_LFLAGS = -fsanitize=leak

.PHONY : all clean fclean re

all: $(NAME)

leak: LFLAGS += $(DEBUG_LFLAGS)
leak: CFLAGS += $(DEBUG_CFLAGS)
leak: $(NAME)

$(NAME): $(_OBJS)
	$(CC) $(_OBJS) $(LFLAGS) -o $(NAME)

$(_OBJS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp $(DIRS)
	$(CC) -c $(CFLAGS) $< -o $@

$(DIRS):
	mkdir -p $(DIRS)

clean:
	rm -rf $(_OBJS)

fclean:
	rm -rf $(_OBJS) $(NAME)

re: fclean all
