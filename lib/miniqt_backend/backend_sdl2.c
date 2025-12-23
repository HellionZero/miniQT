/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend_sdl2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 16:10:37 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend.h"
#include <stdlib.h>

#ifdef MQT_USE_SDL2
# include <SDL2/SDL.h>

/* Estrutura de dados específica do SDL2 */
typedef struct s_sdl2_data
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	SDL_Event		event;
}	t_sdl2_data;

static int	sdl2_init(t_backend_context *ctx, int width, int height,
		const char *title)
{
	t_sdl2_data	*data;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (-1);
	data = malloc(sizeof(t_sdl2_data));
	if (!data)
	{
		SDL_Quit();
		return (-1);
	}
	data->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!data->window)
	{
		free(data);
		SDL_Quit();
		return (-1);
	}
	data->renderer = SDL_CreateRenderer(data->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!data->renderer)
	{
		/* Tenta software renderer como fallback */
		data->renderer = SDL_CreateRenderer(data->window, -1,
				SDL_RENDERER_SOFTWARE);
		if (!data->renderer)
		{
			SDL_DestroyWindow(data->window);
			free(data);
			SDL_Quit();
			return (-1);
		}
	}
	ctx->backend_data = data;
	return (0);
}

static void	sdl2_destroy(t_backend_context *ctx)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	if (data)
	{
		if (data->renderer)
			SDL_DestroyRenderer(data->renderer);
		if (data->window)
			SDL_DestroyWindow(data->window);
		free(data);
	}
	SDL_Quit();
}

static void	sdl2_begin_frame(t_backend_context *ctx)
{
	(void)ctx;
}

static void	sdl2_end_frame(t_backend_context *ctx)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	SDL_RenderPresent(data->renderer);
}

static void	sdl2_clear(t_backend_context *ctx, t_color color)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	SDL_SetRenderDrawColor(data->renderer, color.r, color.g, color.b,
		color.a);
	SDL_RenderClear(data->renderer);
}

static void	sdl2_draw_rect(t_backend_context *ctx, t_rect rect, t_color color)
{
	t_sdl2_data	*data;
	SDL_Rect	sdl_rect;

	data = (t_sdl2_data *)ctx->backend_data;
	sdl_rect.x = rect.x;
	sdl_rect.y = rect.y;
	sdl_rect.w = rect.width;
	sdl_rect.h = rect.height;
	SDL_SetRenderDrawColor(data->renderer, color.r, color.g, color.b,
		color.a);
	SDL_RenderDrawRect(data->renderer, &sdl_rect);
}

static void	sdl2_draw_rect_filled(t_backend_context *ctx, t_rect rect,
		t_color color)
{
	t_sdl2_data	*data;
	SDL_Rect	sdl_rect;

	data = (t_sdl2_data *)ctx->backend_data;
	sdl_rect.x = rect.x;
	sdl_rect.y = rect.y;
	sdl_rect.w = rect.width;
	sdl_rect.h = rect.height;
	SDL_SetRenderDrawColor(data->renderer, color.r, color.g, color.b,
		color.a);
	SDL_RenderFillRect(data->renderer, &sdl_rect);
}

static void	sdl2_draw_text(t_backend_context *ctx, int x, int y,
		const char *text, t_color color)
{
	(void)ctx;
	(void)x;
	(void)y;
	(void)text;
	(void)color;
}

static int	sdl2_poll_event(t_backend_context *ctx, t_event *event)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	if (SDL_PollEvent(&data->event))
	{
		if (data->event.type == SDL_QUIT)
		{
			event->type = MQT_EVENT_QUIT;
			return (1);
		}
		else if (data->event.type == SDL_KEYDOWN)
		{
			event->type = MQT_EVENT_KEY_DOWN;
			event->key_code = data->event.key.keysym.sym;
			return (1);
		}
		else if (data->event.type == SDL_MOUSEBUTTONDOWN)
		{
			event->type = MQT_EVENT_MOUSE_DOWN;
			event->mouse_x = data->event.button.x;
			event->mouse_y = data->event.button.y;
			event->mouse_button = data->event.button.button;
			return (1);
		}
		/* Outros eventos são ignorados */
		return (0);
	}
	return (0);
}

static void	sdl2_set_title(t_backend_context *ctx, const char *title)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	SDL_SetWindowTitle(data->window, title);
}

static void	sdl2_get_size(t_backend_context *ctx, int *width, int *height)
{
	t_sdl2_data	*data;

	data = (t_sdl2_data *)ctx->backend_data;
	SDL_GetWindowSize(data->window, width, height);
}

const t_backend_ops	g_sdl2_backend_ops = {
	.init = sdl2_init,
	.destroy = sdl2_destroy,
	.begin_frame = sdl2_begin_frame,
	.end_frame = sdl2_end_frame,
	.clear = sdl2_clear,
	.draw_rect = sdl2_draw_rect,
	.draw_rect_filled = sdl2_draw_rect_filled,
	.draw_text = sdl2_draw_text,
	.poll_event = sdl2_poll_event,
	.set_title = sdl2_set_title,
	.get_size = sdl2_get_size
};

#else

/* Stub quando SDL2 não está disponível */
const t_backend_ops	g_sdl2_backend_ops = {0};

#endif
