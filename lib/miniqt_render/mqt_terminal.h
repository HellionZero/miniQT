/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_terminal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MQT_TERMINAL_H
# define MQT_TERMINAL_H

# include <unistd.h>
# include <stdio.h>

/* Cores ANSI */
# define ANSI_RESET			"\033[0m"
# define ANSI_BOLD			"\033[1m"
# define ANSI_DIM			"\033[2m"
# define ANSI_CLEAR			"\033[2J"
# define ANSI_HOME			"\033[H"

/* Cores de texto */
# define ANSI_BLACK			"\033[30m"
# define ANSI_RED			"\033[31m"
# define ANSI_GREEN			"\033[32m"
# define ANSI_YELLOW		"\033[33m"
# define ANSI_BLUE			"\033[34m"
# define ANSI_MAGENTA		"\033[35m"
# define ANSI_CYAN			"\033[36m"
# define ANSI_WHITE			"\033[37m"

/* Cores de fundo */
# define ANSI_BG_BLACK		"\033[40m"
# define ANSI_BG_RED		"\033[41m"
# define ANSI_BG_GREEN		"\033[42m"
# define ANSI_BG_YELLOW		"\033[43m"
# define ANSI_BG_BLUE		"\033[44m"
# define ANSI_BG_MAGENTA	"\033[45m"
# define ANSI_BG_CYAN		"\033[46m"
# define ANSI_BG_WHITE		"\033[47m"

/* Cores RGB (256 cores) */
typedef struct s_term_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_term_color;

/* Funções de terminal */
void	mqt_clear_screen(void);
void	mqt_goto_xy(int x, int y);
void	mqt_hide_cursor(void);
void	mqt_show_cursor(void);
void	mqt_set_color(t_term_color fg, t_term_color bg);
void	mqt_reset_color(void);
void	mqt_putchar_at(int x, int y, char c);
void	mqt_putstr_at(int x, int y, const char *str);

/* Funções auxiliares de cores */
t_term_color	mqt_rgb(unsigned char r, unsigned char g, unsigned char b);
void	mqt_print_colored_char(char c, t_term_color fg, t_term_color bg);

#endif
