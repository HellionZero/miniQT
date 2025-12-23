/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend_terminal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 16:01:21 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend.h"
#include "../miniqt_render/mqt_framebuffer.h"
#include "../miniqt_render/mqt_draw.h"
#include <stdlib.h>
#include <unistd.h>

/* Estrutura de dados do backend terminal */
typedef struct s_terminal_data
{
	t_framebuffer	*framebuffer;
}	t_terminal_data;

static int	terminal_init(t_backend_context *ctx, int width, int height,
		const char *title)
{
	t_terminal_data	*data;

	(void)title;
	data = malloc(sizeof(t_terminal_data));
	if (!data)
		return (-1);
	data->framebuffer = mqt_fb_create(width, height);
	if (!data->framebuffer)
	{
		free(data);
		return (-1);
	}
	ctx->backend_data = data;
	mqt_clear_screen();
	mqt_hide_cursor();
	return (0);
}

static void	terminal_destroy(t_backend_context *ctx)
{
	t_terminal_data	*data;

	data = (t_terminal_data *)ctx->backend_data;
	if (data)
	{
		if (data->framebuffer)
			mqt_fb_destroy(data->framebuffer);
		free(data);
	}
	mqt_show_cursor();
	mqt_goto_xy(0, ctx->height + 1);
	write(1, "\n", 1);
}

static void	terminal_begin_frame(t_backend_context *ctx)
{
	(void)ctx;
}

static void	terminal_end_frame(t_backend_context *ctx)
{
	t_terminal_data	*data;

	data = (t_terminal_data *)ctx->backend_data;
	mqt_fb_render(data->framebuffer);
}

static void	terminal_clear(t_backend_context *ctx, t_color color)
{
	t_terminal_data	*data;
	t_term_color	term_color;

	data = (t_terminal_data *)ctx->backend_data;
	term_color = mqt_rgb(color.r, color.g, color.b);
	mqt_fb_clear(data->framebuffer, term_color);
}

static void	terminal_draw_rect(t_backend_context *ctx, t_rect rect,
		t_color color)
{
	t_terminal_data	*data;
	t_pixel			pixel;
	t_term_color	term_color;

	data = (t_terminal_data *)ctx->backend_data;
	term_color = mqt_rgb(color.r, color.g, color.b);
	pixel = mqt_pixel(' ', term_color, term_color);
	mqt_draw_rect(data->framebuffer, rect, pixel);
}

static void	terminal_draw_rect_filled(t_backend_context *ctx, t_rect rect,
		t_color color)
{
	t_terminal_data	*data;
	t_pixel			pixel;
	t_term_color	term_color;

	data = (t_terminal_data *)ctx->backend_data;
	term_color = mqt_rgb(color.r, color.g, color.b);
	pixel = mqt_pixel(' ', term_color, term_color);
	mqt_draw_rect_filled(data->framebuffer, rect, pixel);
}

static void	terminal_draw_text(t_backend_context *ctx, int x, int y,
		const char *text, t_color color)
{
	t_terminal_data	*data;
	t_term_color	term_fg;
	t_term_color	term_bg;

	data = (t_terminal_data *)ctx->backend_data;
	term_fg = mqt_rgb(color.r, color.g, color.b);
	term_bg = mqt_rgb(0, 0, 0);
	mqt_draw_text(data->framebuffer, x, y, text, term_fg, term_bg);
}

static int	terminal_poll_event(t_backend_context *ctx, t_event *event)
{
	(void)ctx;
	(void)event;
	return (0);
}

static void	terminal_set_title(t_backend_context *ctx, const char *title)
{
	(void)ctx;
	(void)title;
}

static void	terminal_get_size(t_backend_context *ctx, int *width, int *height)
{
	*width = ctx->width;
	*height = ctx->height;
}

const t_backend_ops	g_terminal_backend_ops = {
	.init = terminal_init,
	.destroy = terminal_destroy,
	.begin_frame = terminal_begin_frame,
	.end_frame = terminal_end_frame,
	.clear = terminal_clear,
	.draw_rect = terminal_draw_rect,
	.draw_rect_filled = terminal_draw_rect_filled,
	.draw_text = terminal_draw_text,
	.poll_event = terminal_poll_event,
	.set_title = terminal_set_title,
	.get_size = terminal_get_size
};
