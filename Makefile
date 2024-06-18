BUIN	= cd echo env exit export export1 pwd unset

EXE		= exe_all exe_and exe_one exe_or exe_pipe

# RDIR	= io resetio heredoc

# PIPE	= setpip

# RLOD	= dequo quo quo1 loadio

# SIG		= signal signal1

# SPLT	= splitcmd eachcmd

UTIL	= free ft_printf ft_split strjoin strjoin2 sutils sutils1 utils split readline

INIT	= set setnoenv

PRSE	= load_var reload dequo load_wikd

CHCK	= check

SRCS	= $(addsuffix .c, $(addprefix builtin/, $(BUIN))) \
		$(addsuffix .c, $(addprefix init/, $(INIT))) \
		$(addsuffix .c, $(addprefix redir/, $(RDIR))) \
		$(addsuffix .c, $(addprefix reloadcmd/, $(RLOD))) \
		$(addsuffix .c, $(addprefix signal/, $(SIG))) \
		$(addsuffix .c, $(addprefix splitcmd/, $(SPLT))) \
		$(addsuffix .c, $(addprefix utils/, $(UTIL))) \
		$(addsuffix .c, $(addprefix check/, $(CHCK))) \
		$(addsuffix .c, $(addprefix exe/, $(EXE))) \
		$(addsuffix .c, $(addprefix pipe/, $(PIPE))) \
		$(addsuffix .c, $(addprefix parse/, $(PRSE))) \
		main.c

OBJS	= $(SRCS:.c=.o)
INCL	= ./
NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror -I$(INCL)
CC		= cc

all:$(NAME)

$(NAME):$(OBJS)
		$(CC) $(CFLAGS) -lreadline $(OBJS) -o $(NAME)
		$(CC) $(CFLAGS) -lreadline $(OBJS) -o minishell_bonus

re:fclean all

fclean:clean
		rm -f $(NAME)

clean:
		rm -f $(OBJS)

.PHONY: clean fclean all re
