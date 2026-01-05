/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/05 17:41:23 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Exemplo de uso do sistema de backends MiniQT
 * 
 * Este programa demonstra como:
 * - Usar a factory para criar backend automaticamente
 * - Desenhar formas e texto
 * - Processar eventos
 * - Alternar entre backends em tempo de compilação
 */

#include "../lib/miniqt_backend/backend_factory.h"
#include <stdio.h>
#include <unistd.h>

void	draw_demo_content(t_backend_context *ctx)
{
	t_color	bg;
	t_color	blue;
	t_color	red;
	t_color	green;
	t_color	white;

	bg = mqt_color(20, 20, 30, 255);
	blue = mqt_color(70, 130, 220, 255);
	red = mqt_color(220, 70, 70, 255);
	green = mqt_color(70, 220, 70, 255);
	white = mqt_color(255, 255, 255, 255);

	mqt_begin_frame(ctx);
	mqt_clear_screen_color(ctx, bg);

	/* Desenhar retângulos coloridos */
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(50, 50, 200, 100), blue);
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(300, 50, 200, 100), red);
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(550, 50, 200, 100), green);

	/* Desenhar bordas */
	mqt_draw_rectangle(ctx, mqt_make_rect(50, 200, 200, 100), white);
	mqt_draw_rectangle(ctx, mqt_make_rect(300, 200, 200, 100), white);
	mqt_draw_rectangle(ctx, mqt_make_rect(550, 200, 200, 100), white);

	/* Desenhar texto */
	mqt_draw_string(ctx, 320, 350, "MiniQT Cross-Platform Demo", white);
	mqt_draw_string(ctx, 280, 370, "Press ESC or close window to exit", white);

	mqt_end_frame(ctx);
}

int	main(int argc, char **argv)
{
	t_backend_context	*ctx;
	t_event				event;
	int					running;

	/* Listar backends disponíveis */
	mqt_list_available_backends();
	printf("\n");

	/* Criar backend automaticamente */
	ctx = mqt_create_auto_backend(800, 400, "MiniQT Demo");
	if (!ctx)
	{
		fprintf(stderr, "Failed to create backend\n");
		return (1);
	}

	printf("\n[Demo] Running... (press ESC to exit)\n\n");

	/* Loop principal */
	running = 1;
	while (running)
	{
		/* Processar eventos */
		while (mqt_poll_event_ctx(ctx, &event))
		{
			if (event.type == MQT_EVENT_QUIT)
				running = 0;
			else if (event.type == MQT_EVENT_KEY_DOWN && event.key_code == 27)
				running = 0; /* ESC key */
		}

		/* Desenhar */
		draw_demo_content(ctx);

		/* Para backend terminal, pausa para não consumir CPU */
		if (ctx->backend_type == MQT_BACKEND_TERMINAL)
		{
			sleep(5);
			running = 0; /* Terminal: executa uma vez e sai */
		}
		else
			usleep(16666); /* ~60 FPS */
	}

	mqt_backend_destroy(ctx);
	printf("[Demo] Exited cleanly\n");
	return (0);
}
