/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Declarações de backends específicos */
extern const t_backend_ops	g_terminal_backend_ops;
extern const t_backend_ops	g_sdl2_backend_ops;

static const t_backend_ops	*get_backend_ops(t_graphics_backend type)
{
	if (type == MQT_BACKEND_TERMINAL)
		return (&g_terminal_backend_ops);
	else if (type == MQT_BACKEND_SDL2)
		return (&g_sdl2_backend_ops);
	return (NULL);
}

t_backend_context	*mqt_backend_create(t_graphics_backend backend_type,
		int width, int height, const char *title)
{
	t_backend_context	*ctx;

	ctx = malloc(sizeof(t_backend_context));
	if (!ctx)
		return (NULL);
	ctx->backend_type = backend_type;
	ctx->ops = get_backend_ops(backend_type);
	ctx->backend_data = NULL;
	ctx->width = width;
	ctx->height = height;
	ctx->title = strdup(title ? title : "MiniQT Window");
	if (!ctx->title)
	{
		free(ctx);
		return (NULL);
	}
	ctx->is_running = 1;
	if (!ctx->ops)
	{
		fprintf(stderr, "[Backend Error] No ops for backend type %d\n", backend_type);
		free(ctx->title);
		free(ctx);
		return (NULL);
	}
	printf("[Backend] Calling init function for backend...\n");
	fflush(stdout);
	if (ctx->ops->init && ctx->ops->init(ctx, width, height, title) != 0)
	{
		fprintf(stderr, "[Backend Error] init function failed\n");
		free(ctx->title);
		free(ctx);
		return (NULL);
	}
	printf("[Backend] Init successful!\n");
	return (ctx);
}

void	mqt_backend_destroy(t_backend_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->ops && ctx->ops->destroy)
		ctx->ops->destroy(ctx);
	if (ctx->title)
		free(ctx->title);
	free(ctx);
}

t_color	mqt_color(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
{
	t_color	c;

	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return (c);
}

t_rect	mqt_make_rect(int x, int y, int width, int height)
{
	t_rect	r;

	r.x = x;
	r.y = y;
	r.width = width;
	r.height = height;
	return (r);
}
