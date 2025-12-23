/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_framebuffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mqt_framebuffer.h"

t_framebuffer	*mqt_fb_create(int width, int height)
{
	t_framebuffer	*fb;
	int				y;

	fb = malloc(sizeof(t_framebuffer));
	if (!fb)
		return (NULL);
	fb->width = width;
	fb->height = height;
	fb->pixels = malloc(sizeof(t_pixel *) * height);
	if (!fb->pixels)
	{
		free(fb);
		return (NULL);
	}
	y = 0;
	while (y < height)
	{
		fb->pixels[y] = malloc(sizeof(t_pixel) * width);
		if (!fb->pixels[y])
		{
			while (--y >= 0)
				free(fb->pixels[y]);
			free(fb->pixels);
			free(fb);
			return (NULL);
		}
		y++;
	}
	return (fb);
}

void	mqt_fb_destroy(t_framebuffer *fb)
{
	int	y;

	if (!fb)
		return ;
	if (fb->pixels)
	{
		y = 0;
		while (y < fb->height)
		{
			free(fb->pixels[y]);
			y++;
		}
		free(fb->pixels);
	}
	free(fb);
}

void	mqt_fb_clear(t_framebuffer *fb, t_term_color bg_color)
{
	int		x;
	int		y;
	t_pixel	empty;

	empty = mqt_pixel(' ', mqt_rgb(0, 0, 0), bg_color);
	y = 0;
	while (y < fb->height)
	{
		x = 0;
		while (x < fb->width)
		{
			fb->pixels[y][x] = empty;
			x++;
		}
		y++;
	}
}

void	mqt_fb_render(t_framebuffer *fb)
{
	int		x;
	int		y;
	t_pixel	*current;

	mqt_hide_cursor();
	mqt_goto_xy(0, 0);
	y = 0;
	while (y < fb->height)
	{
		x = 0;
		while (x < fb->width)
		{
			current = &fb->pixels[y][x];
			mqt_set_color(current->fg_color, current->bg_color);
			write(1, &current->character, 1);
			x++;
		}
		if (y < fb->height - 1)
			write(1, "\n", 1);
		y++;
	}
	mqt_reset_color();
}

void	mqt_fb_set_pixel(t_framebuffer *fb, int x, int y, t_pixel pixel)
{
	if (x >= 0 && x < fb->width && y >= 0 && y < fb->height)
		fb->pixels[y][x] = pixel;
}

t_pixel	mqt_fb_get_pixel(t_framebuffer *fb, int x, int y)
{
	if (x >= 0 && x < fb->width && y >= 0 && y < fb->height)
		return (fb->pixels[y][x]);
	return (mqt_pixel(' ', mqt_rgb(0, 0, 0), mqt_rgb(0, 0, 0)));
}

t_pixel	mqt_pixel(char c, t_term_color fg, t_term_color bg)
{
	t_pixel	pixel;

	pixel.character = c;
	pixel.fg_color = fg;
	pixel.bg_color = bg;
	return (pixel);
}
