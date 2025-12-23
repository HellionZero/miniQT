/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 15:58:10 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 16:09:46 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Demo básico da MiniQT API
 * Mostra desenho de formas e cores sem interação
 */

#include "../../lib/miniqt_backend/backend_factory.h"
#include <stdio.h>
#include <unistd.h>

void	draw_colorful_boxes(t_backend_context *ctx)
{
	t_color	colors[6];
	t_rect	box;
	int		i;

	colors[0] = mqt_color(255, 100, 100, 255);
	colors[1] = mqt_color(100, 255, 100, 255);
	colors[2] = mqt_color(100, 100, 255, 255);
	colors[3] = mqt_color(255, 255, 100, 255);
	colors[4] = mqt_color(255, 100, 255, 255);
	colors[5] = mqt_color(100, 255, 255, 255);
	i = 0;
	while (i < 6)
	{
		box = mqt_make_rect(50 + i * 120, 200, 100, 80);
		mqt_draw_rectangle_filled(ctx, box, colors[i]);
		mqt_draw_rectangle(ctx, box, mqt_color(255, 255, 255, 255));
		i++;
	}
}

void	draw_scene(t_backend_context *ctx)
{
	t_color	bg;
	t_color	blue;
	t_color	red;
	t_color	white;

	bg = mqt_color(20, 20, 30, 255);
	blue = mqt_color(70, 130, 220, 255);
	red = mqt_color(220, 70, 70, 255);
	white = mqt_color(255, 255, 255, 255);
	mqt_begin_frame(ctx);
	mqt_clear_screen_color(ctx, bg);
	mqt_draw_string(ctx, 280, 30, "MiniQT - Demo Básico", white);
	mqt_draw_string(ctx, 250, 50, "Formas geométricas e cores", white);
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(50, 100, 300, 80), blue);
	mqt_draw_rectangle(ctx, mqt_make_rect(50, 100, 300, 80), white);
	mqt_draw_string(ctx, 130, 130, "Retângulo Azul", white);
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(400, 100, 300, 80), red);
	mqt_draw_rectangle(ctx, mqt_make_rect(400, 100, 300, 80), white);
	mqt_draw_string(ctx, 480, 130, "Retângulo Vermelho", white);
	draw_colorful_boxes(ctx);
	mqt_draw_string(ctx, 250, 320, "Paleta de 6 cores", white);
	mqt_draw_string(ctx, 600, 550, "ESC para sair",
		mqt_color(150, 150, 150, 255));
	mqt_end_frame(ctx);
}

int	main(void)
{
	t_backend_context	*ctx;
	t_event				event;
	int					running;

	printf("[Demo] Iniciando demo básico...\n");
	mqt_list_available_backends();
	printf("\n");
	ctx = mqt_create_auto_backend(800, 400, "MiniQT - Basic Demo");
	if (!ctx)
	{
		fprintf(stderr, "[Erro] Falha ao criar backend\n");
		return (1);
	}
	printf("[Demo] Backend: %s\n", mqt_backend_name(ctx->backend_type));
	running = 1;
	while (running)
	{
		while (mqt_poll_event_ctx(ctx, &event))
		{
			if (event.type == MQT_EVENT_QUIT)
				running = 0;
			else if (event.type == MQT_EVENT_KEY_DOWN && event.key_code == 27)
				running = 0;
		}
		draw_scene(ctx);
		if (ctx->backend_type == MQT_BACKEND_TERMINAL)
		{
			sleep(5);
			running = 0;
		}
		else
			usleep(16666);
	}
	mqt_backend_destroy(ctx);
	printf("[Demo] Finalizado!\n");
	return (0);
}
