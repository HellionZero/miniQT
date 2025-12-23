/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_terminal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mqt_terminal.h"

void	mqt_clear_screen(void)
{
	write(1, ANSI_CLEAR, 4);
	write(1, ANSI_HOME, 3);
}

void	mqt_goto_xy(int x, int y)
{
	char	buffer[32];
	int		len;

	len = snprintf(buffer, sizeof(buffer), "\033[%d;%dH", y + 1, x + 1);
	write(1, buffer, len);
}

void	mqt_hide_cursor(void)
{
	write(1, "\033[?25l", 6);
}

void	mqt_show_cursor(void)
{
	write(1, "\033[?25h", 6);
}

void	mqt_reset_color(void)
{
	write(1, ANSI_RESET, 4);
}

void	mqt_set_color(t_term_color fg, t_term_color bg)
{
	char	buffer[64];
	int		len;

	len = snprintf(buffer, sizeof(buffer),
			"\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",
			fg.r, fg.g, fg.b, bg.r, bg.g, bg.b);
	write(1, buffer, len);
}

t_term_color	mqt_rgb(unsigned char r, unsigned char g, unsigned char b)
{
	t_term_color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	return (color);
}

void	mqt_putchar_at(int x, int y, char c)
{
	mqt_goto_xy(x, y);
	write(1, &c, 1);
}

void	mqt_putstr_at(int x, int y, const char *str)
{
	mqt_goto_xy(x, y);
	while (*str)
		write(1, str++, 1);
}

void	mqt_print_colored_char(char c, t_term_color fg, t_term_color bg)
{
	mqt_set_color(fg, bg);
	write(1, &c, 1);
	mqt_reset_color();
}
