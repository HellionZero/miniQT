/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mqt_draw.h"

t_rect	mqt_rect(int x, int y, int width, int height)
{
	t_rect	rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	return (rect);
}

void	mqt_draw_rect_filled(t_framebuffer *fb, t_rect rect, t_pixel pixel)
{
	int	x;
	int	y;

	y = rect.y;
	while (y < rect.y + rect.height)
	{
		x = rect.x;
		while (x < rect.x + rect.width)
		{
			mqt_fb_set_pixel(fb, x, y, pixel);
			x++;
		}
		y++;
	}
}

void	mqt_draw_rect(t_framebuffer *fb, t_rect rect, t_pixel pixel)
{
	int	x;
	int	y;

	x = rect.x;
	while (x < rect.x + rect.width)
	{
		mqt_fb_set_pixel(fb, x, rect.y, pixel);
		mqt_fb_set_pixel(fb, x, rect.y + rect.height - 1, pixel);
		x++;
	}
	y = rect.y;
	while (y < rect.y + rect.height)
	{
		mqt_fb_set_pixel(fb, rect.x, y, pixel);
		mqt_fb_set_pixel(fb, rect.x + rect.width - 1, y, pixel);
		y++;
	}
}

void	mqt_draw_line_h(t_framebuffer *fb, int x, int y, int length,
		t_pixel pixel)
{
	int	i;

	i = 0;
	while (i < length)
	{
		mqt_fb_set_pixel(fb, x + i, y, pixel);
		i++;
	}
}

void	mqt_draw_line_v(t_framebuffer *fb, int x, int y, int length,
		t_pixel pixel)
{
	int	i;

	i = 0;
	while (i < length)
	{
		mqt_fb_set_pixel(fb, x, y + i, pixel);
		i++;
	}
}

void	mqt_draw_text(t_framebuffer *fb, int x, int y, const char *text,
		t_term_color fg, t_term_color bg)
{
	int		i;
	t_pixel	pixel;

	i = 0;
	while (text[i])
	{
		pixel = mqt_pixel(text[i], fg, bg);
		mqt_fb_set_pixel(fb, x + i, y, pixel);
		i++;
	}
}

void	mqt_draw_border(t_framebuffer *fb, t_rect rect, t_term_color fg, t_term_color bg)
{
	t_pixel	corner;
	t_pixel	horiz;
	t_pixel	vert;
	int		x;
	int		y;

	corner = mqt_pixel('+', fg, bg);
	horiz = mqt_pixel('-', fg, bg);
	vert = mqt_pixel('|', fg, bg);
	mqt_fb_set_pixel(fb, rect.x, rect.y, corner);
	mqt_fb_set_pixel(fb, rect.x + rect.width - 1, rect.y, corner);
	mqt_fb_set_pixel(fb, rect.x, rect.y + rect.height - 1, corner);
	mqt_fb_set_pixel(fb, rect.x + rect.width - 1, rect.y + rect.height - 1,
		corner);
	x = rect.x + 1;
	while (x < rect.x + rect.width - 1)
	{
		mqt_fb_set_pixel(fb, x, rect.y, horiz);
		mqt_fb_set_pixel(fb, x, rect.y + rect.height - 1, horiz);
		x++;
	}
	y = rect.y + 1;
	while (y < rect.y + rect.height - 1)
	{
		mqt_fb_set_pixel(fb, rect.x, y, vert);
		mqt_fb_set_pixel(fb, rect.x + rect.width - 1, y, vert);
		y++;
	}
}
