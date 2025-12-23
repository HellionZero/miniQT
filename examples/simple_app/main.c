/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Aplicação simples - Template para novos projetos
 * Exemplo mínimo de como usar a MiniQT API
 */

#include "../../lib/miniqt_backend/backend_factory.h"
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	t_backend_context	*ctx;
	t_event				event;
	int					running;

	// Criar contexto gráfico (escolhe backend automaticamente)
	ctx = mqt_create_auto_backend(640, 480, "Minha Aplicação");
	if (!ctx)
	{
		fprintf(stderr, "Erro: Não foi possível criar contexto gráfico\n");
		return (1);
	}
	printf("Backend: %s\n", mqt_backend_name(ctx->backend_type));
	running = 1;
	while (running)
	{
		// Processar eventos
		while (mqt_poll_event_ctx(ctx, &event))
		{
			if (event.type == MQT_EVENT_QUIT)
				running = 0;
			if (event.type == MQT_EVENT_KEY_DOWN)
			{
				if (event.key_code == 27)
					running = 0;
			}
		}
		// Renderizar frame
		mqt_begin_frame(ctx);
		mqt_clear_screen_color(ctx, mqt_color(25, 25, 35, 255));
		mqt_draw_string(ctx, 240, 200, "Olá, MiniQT!",
			mqt_color(255, 255, 255, 255));
		mqt_draw_string(ctx, 200, 230, "Pressione ESC para sair",
			mqt_color(150, 150, 150, 255));
		mqt_draw_rectangle_filled(ctx, mqt_make_rect(220, 270, 200, 100),
			mqt_color(80, 120, 200, 255));
		mqt_draw_string(ctx, 260, 310, "MiniQT API",
			mqt_color(255, 255, 255, 255));
		mqt_end_frame(ctx);
		// Controle de FPS
		if (ctx->backend_type == MQT_BACKEND_TERMINAL)
		{
			sleep(3);
			running = 0;
		}
		else
			usleep(16666);
	}
	mqt_backend_destroy(ctx);
	printf("Aplicação finalizada.\n");
	return (0);
}
