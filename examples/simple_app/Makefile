# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/23 15:58:41 by lsarraci          #+#    #+#              #
#    Updated: 2025/12/23 15:58:45 by lsarraci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= simple_app

# Diretórios
MINIQT_ROOT	= ../..
LIB_DIR		= $(MINIQT_ROOT)/lib
INC_DIR		= $(MINIQT_ROOT)/include

# Arquivo fonte
SRCS		= main.c
OBJS		= $(SRCS:.c=.o)

# Bibliotecas MiniQT
LIBFT		= $(LIB_DIR)/libft/libft.a
PRINTF		= $(LIB_DIR)/ft_printf/libftprintf.a

# Backend sources
BACKEND_DIR		= $(LIB_DIR)/miniqt_backend
BACKEND_SRCS	= $(BACKEND_DIR)/platform.c \
				  $(BACKEND_DIR)/backend.c \
				  $(BACKEND_DIR)/backend_terminal.c \
				  $(BACKEND_DIR)/backend_sdl2.c \
				  $(BACKEND_DIR)/backend_factory.c

# Render sources
RENDER_DIR		= $(LIB_DIR)/miniqt_render
RENDER_SRCS		= $(RENDER_DIR)/mqt_terminal.c \
				  $(RENDER_DIR)/mqt_framebuffer.c \
				  $(RENDER_DIR)/mqt_draw.c

# Todos os objetos da API
API_SRCS		= $(BACKEND_SRCS) $(RENDER_SRCS)
API_OBJS		= $(API_SRCS:.c=.o)

# Compilador e flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
CFLAGS		+= -I$(INC_DIR)
CFLAGS		+= -I$(BACKEND_DIR)
CFLAGS		+= -I$(RENDER_DIR)
CFLAGS		+= -I$(LIB_DIR)/libft

# SDL2 (opcional - usa pkg-config se disponível)
ifdef SDL2
	# Tenta usar SDL2 local primeiro, depois do sistema
	ifeq ($(wildcard $(LIB_DIR)/SDL2/install/lib/libSDL2.a),)
		# SDL2 do sistema via pkg-config
		SDL2_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null || echo "-I/usr/include/SDL2")
		SDL2_LIBS := $(shell pkg-config --libs sdl2 2>/dev/null || echo "-lSDL2")
	else
		# SDL2 local compilado
		SDL2_CFLAGS := -I$(LIB_DIR)/SDL2/install/include/SDL2
		SDL2_LIBS := -L$(LIB_DIR)/SDL2/install/lib -lSDL2 -Wl,-rpath,$(LIB_DIR)/SDL2/install/lib
	endif
	CFLAGS += -DMQT_USE_SDL2 $(SDL2_CFLAGS)
	LDFLAGS += $(SDL2_LIBS)
endif

# Cores
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
RESET	= \033[0m

all: $(LIBFT) $(PRINTF) $(NAME)

$(NAME): $(OBJS) $(API_OBJS)
	@echo "$(YELLOW)Linkando $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(API_OBJS) $(LIBFT) $(PRINTF) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) criado!$(RESET)"

%.o: %.c
	@echo "$(YELLOW)Compilando $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIB_DIR)/libft --no-print-directory

$(PRINTF):
	@make -C $(LIB_DIR)/ft_printf --no-print-directory

clean:
	@echo "$(RED)Limpando objetos...$(RESET)"
	@rm -f $(OBJS) $(API_OBJS)

fclean: clean
	@echo "$(RED)Removendo $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

run: $(NAME)
	@echo "$(GREEN)Executando $(NAME)...$(RESET)"
	@./$(NAME)

.PHONY: all clean fclean re run
