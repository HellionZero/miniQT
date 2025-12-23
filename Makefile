# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#              #
#    Updated: 2025/12/23 15:36:23 by lsarraci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= miniqt

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
INC_DIR		= include
LIB_DIR		= lib

# Source files
SRCS		= $(SRC_DIR)/main.c

# Object files
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libraries
LIBFT_DIR	= $(LIB_DIR)/libft
LIBFT		= $(LIBFT_DIR)/libft.a

PRINTF_DIR	= $(LIB_DIR)/ft_printf
PRINTF		= $(PRINTF_DIR)/libftprintf.a

RENDER_DIR	= $(LIB_DIR)/miniqt_render
RENDER_SRCS	= $(RENDER_DIR)/mqt_terminal.c \
			  $(RENDER_DIR)/mqt_framebuffer.c \
			  $(RENDER_DIR)/mqt_draw.c \
			  $(RENDER_DIR)/mqt_window.c
RENDER_OBJS	= $(RENDER_SRCS:$(RENDER_DIR)/%.c=$(OBJ_DIR)/render/%.o)

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBFT_DIR) -I$(RENDER_DIR)
LDFLAGS		= -L$(LIBFT_DIR) -lft -L$(PRINTF_DIR) -lftprintf

# Colors
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(PRINTF) $(OBJS) $(RENDER_OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(RENDER_OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/render/%.o: $(RENDER_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/render
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(YELLOW)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR) --no-print-directory
	@echo "$(GREEN)✓ libft built$(RESET)"

$(PRINTF):
	@echo "$(YELLOW)Building ft_printf...$(RESET)"
	@make -C $(PRINTF_DIR) --no-print-directory
	@echo "$(GREEN)✓ ft_printf built$(RESET)"

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@make -C $(PRINTF_DIR) clean --no-print-directory
	@echo "$(GREEN)✓ Clean complete$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@make -C $(PRINTF_DIR) fclean --no-print-directory
	@echo "$(GREEN)✓ Full clean complete$(RESET)"

re: fclean all

run: $(NAME)
	@echo "$(GREEN)Running $(NAME)...$(RESET)"
	@./$(NAME)

.PHONY: all clean fclean re run
