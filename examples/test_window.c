/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2026/01/02 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniqt.h"
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	t_backend_context	*ctx;
	t_event				event;
	t_color				bg;
	t_color				rect_color;
	t_rect				rect;
	int					running;
	int					x;

	printf("\n=== Teste de Janela SDL2 via SSH ===\n\n");
	
	/* Cria o backend automático */
	ctx = mqt_create_auto_backend(800, 600, "Teste SSH - miniQT");
	if (!ctx)
	{
		printf("❌ Erro ao criar backend!\n");
		return (1);
	}
	
	printf("✓ Janela criada com sucesso!\n");
	printf("✓ Pressione ESC ou feche a janela para sair\n\n");
	
	/* Cores */
	bg.r = 30;
	bg.g = 30;
	bg.b = 40;
	bg.a = 255;
	
	rect_color.r = 100;
	rect_color.g = 200;
	rect_color.b = 255;
	rect_color.a = 255;
	
	/* Retângulo animado */
	rect.width = 100;
	rect.height = 100;
	rect.y = 250;
	x = 0;
	
	running = 1;
	while (running)
	{
		/* Processa eventos */
		while (mqt_backend_poll_event(ctx, &event))
		{
			if (event.type == MQT_EVENT_QUIT)
			{
				printf("🚪 Evento QUIT recebido\n");
				running = 0;
			}
			else if (event.type == MQT_EVENT_KEY_DOWN)
			{
				if (event.key_code == 27) /* ESC */
				{
					printf("⌨️  ESC pressionado\n");
					running = 0;
				}
			}
			else if (event.type == MQT_EVENT_MOUSE_DOWN)
			{
				printf("🖱️  Mouse clicado em (%d, %d)\n",
					event.mouse_x, event.mouse_y);
			}
		}
		
		/* Desenha */
		mqt_backend_begin_frame(ctx);
		mqt_backend_clear(ctx, bg);
		
		/* Retângulo animado */
		rect.x = x;
		mqt_backend_draw_rect_filled(ctx, rect, rect_color);
		
		/* Texto informativo */
		t_color text_color;
		text_color.r = 255;
		text_color.g = 255;
		text_color.b = 255;
		text_color.a = 255;
		
		mqt_backend_draw_text(ctx, 10, 10, "miniQT - Teste SSH", text_color);
		mqt_backend_draw_text(ctx, 10, 30, "Pressione ESC para sair", text_color);
		
		mqt_backend_end_frame(ctx);
		
		/* Animação simples */
		x += 2;
		if (x > 700)
			x = 0;
		
		/* FPS cap ~60 */
		usleep(16000);
	}
	
	printf("\n✓ Encerrando aplicação...\n");
	mqt_backend_destroy(ctx);
	printf("✓ Janela destruída com sucesso!\n");
	printf("✓ Sessão finalizada sem deixar rastros\n\n");
	
	return (0);
}
