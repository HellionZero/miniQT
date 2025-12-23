/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:30:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 16:30:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Demo básico da MiniQT API - VERSÃO TERMINAL FORÇADA
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
		box = mqt_make_rect(5 + i * 12, 20, 10, 4);
		mqt_draw_rectangle_filled(ctx, box, colors[i]);
		i++;
	}
}

void	draw_scene(t_backend_context *ctx)
{
	t_color	bg;
	t_color	blue;
	t_color	red;
	t_color	white;
	t_color	gray;

	bg = mqt_color(20, 20, 30, 255);
	blue = mqt_color(70, 130, 220, 255);
	red = mqt_color(220, 70, 70, 255);
	white = mqt_color(255, 255, 255, 255);
	gray = mqt_color(150, 150, 150, 255);
	
	mqt_begin_frame(ctx);
	mqt_clear_screen_color(ctx, bg);
	
	mqt_draw_string(ctx, 25, 2, "=== MiniQT - Demo Terminal ===", white);
	
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(5, 5, 30, 8), blue);
	mqt_draw_string(ctx, 10, 8, "Retangulo Azul", white);
	
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(40, 5, 30, 8), red);
	mqt_draw_string(ctx, 45, 8, "Retangulo Vermelho", white);
	
	draw_colorful_boxes(ctx);
	mqt_draw_string(ctx, 25, 25, "Paleta de 6 cores", white);
	
	mqt_draw_string(ctx, 50, 27, "Pressione Ctrl+C para sair", gray);
	
	mqt_end_frame(ctx);
}

int	main(void)
{
	t_backend_context	*ctx;

	printf("[Demo Terminal] Iniciando...\n");
	mqt_list_available_backends();
	printf("\n[Demo] Forçando backend TERMINAL\n\n");
	
	/* Força o uso do backend Terminal */
	ctx = mqt_create_backend(MQT_BACKEND_TERMINAL, 80, 30, "MiniQT Terminal Demo");
	if (!ctx)
	{
		fprintf(stderr, "[Erro] Falha ao criar backend terminal\n");
		return (1);
	}
	
	printf("[Demo] Backend: %s\n", mqt_backend_name(ctx->backend_type));
	printf("[Demo] Renderizando...\n\n");
	
	/* Desenha uma única vez e aguarda */
	draw_scene(ctx);
	sleep(5);
	
	mqt_backend_destroy(ctx);
	printf("\n[Demo] Finalizado!\n");
	return (0);
}
