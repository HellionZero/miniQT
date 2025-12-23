# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/23 15:57:02 by lsarraci          #+#    #+#              #
#    Updated: 2025/12/23 15:57:03 by lsarraci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



# MiniQT Examples - Master Makefile
# Compila todos os exemplos da API

.PHONY: all clean fclean re help basic widget simple

all: basic widget simple

basic:
	@echo "$(YELLOW)Compilando Basic Demo...$(RESET)"
	@make -C basic_demo

widget:
	@echo "$(YELLOW)Compilando Widget Demo...$(RESET)"
	@make -C widget_demo

simple:
	@echo "$(YELLOW)Compilando Simple App...$(RESET)"
	@make -C simple_app

clean:
	@echo "$(RED)Limpando exemplos...$(RESET)"
	@make -C basic_demo clean
	@make -C widget_demo clean
	@make -C simple_app clean

fclean:
	@echo "$(RED)Limpeza completa dos exemplos...$(RESET)"
	@make -C basic_demo fclean
	@make -C widget_demo fclean
	@make -C simple_app fclean

re: fclean all

help:
	@echo "=========================================="
	@echo "  MiniQT Examples - Opções de Build"
	@echo "=========================================="
	@echo ""
	@echo "Targets disponíveis:"
	@echo "  make all        - Compila todos os exemplos"
	@echo "  make basic      - Compila Basic Demo"
	@echo "  make widget     - Compila Widget Demo"
	@echo "  make simple     - Compila Simple App"
	@echo "  make clean      - Remove objetos"
	@echo "  make fclean     - Remove tudo"
	@echo "  make re         - Recompila tudo"
	@echo ""
	@echo "Compilar com SDL2:"
	@echo "  make SDL2=1     - Habilita backend SDL2"
	@echo ""
	@echo "Executar exemplos:"
	@echo "  ./basic_demo/basic_demo"
	@echo "  ./widget_demo/widget_demo"
	@echo "  ./simple_app/simple_app"
	@echo ""

# Cores
YELLOW	= \033[0;33m
RED		= \033[0;31m
RESET	= \033[0m
