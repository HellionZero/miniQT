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

#include "../../lib/miniqt_backend/backend_factory.h"
#include "../../lib/miniqt_widgets/button.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	on_button_ok_click(t_button *btn)
{
	printf("[Callback] Botão OK clicado!\n");
	(void)btn;
}

void	on_button_cancel_click(t_button *btn)
{
	printf("[Callback] Botão Cancelar clicado!\n");
	(void)btn;
}

void	on_button_toggle_click(t_button *btn)
{
	static int	enabled = 1;

	enabled = !enabled;
	mqt_button_set_enabled((t_button *)btn->user_data, enabled);
	printf("[Callback] Botão alternado! Estado: %s\n",
		enabled ? "Habilitado" : "Desabilitado");
}

void	draw_ui(t_backend_context *ctx, t_button **buttons, int btn_count)
{
	int		i;
	t_color	bg;
	t_color	white;

	bg = mqt_color(30, 30, 40, 255);
	white = mqt_color(255, 255, 255, 255);
	mqt_begin_frame(ctx);
	mqt_clear_screen_color(ctx, bg);
	mqt_draw_string(ctx, 20, 20, "MiniQT - Widget Demo", white);
	mqt_draw_string(ctx, 20, 40, "Move o mouse e clique nos botoes", white);
	mqt_draw_rectangle_filled(ctx, mqt_make_rect(10, 70, 780, 520),
		mqt_color(40, 40, 50, 255));
	mqt_draw_rectangle(ctx, mqt_make_rect(10, 70, 780, 520),
		mqt_color(100, 150, 200, 255));
	i = 0;
	while (i < btn_count)
	{
		mqt_button_draw(ctx, buttons[i]);
		i++;
	}
	mqt_draw_string(ctx, 600, 560, "ESC para sair",
		mqt_color(150, 150, 150, 255));
	mqt_end_frame(ctx);
}

t_button	**create_buttons(void)
{
	t_button	**buttons;

	buttons = malloc(sizeof(t_button *) * 4);
	if (!buttons)
		return (NULL);
	buttons[0] = mqt_button_create("OK", 50, 150, 150, 50);
	buttons[1] = mqt_button_create("Cancelar", 220, 150, 150, 50);
	buttons[2] = mqt_button_create("Alternar OK", 390, 150, 150, 50);
	buttons[3] = mqt_button_create("Desabilitado", 560, 150, 150, 50);
	if (!buttons[0] || !buttons[1] || !buttons[2] || !buttons[3])
		return (NULL);
	buttons[0]->on_click = on_button_ok_click;
	buttons[1]->on_click = on_button_cancel_click;
	buttons[2]->on_click = on_button_toggle_click;
	buttons[2]->user_data = buttons[0];
	mqt_button_set_enabled(buttons[3], 0);
	mqt_button_set_colors(buttons[1], mqt_color(200, 80, 80, 255),
		mqt_color(220, 100, 100, 255), mqt_color(180, 60, 60, 255));
	return (buttons);
}

void	handle_events(t_backend_context *ctx, t_button **buttons,
		int *running, int *mouse_x, int *mouse_y, int *mouse_down)
{
	t_event	event;

	while (mqt_poll_event_ctx(ctx, &event))
	{
		if (event.type == MQT_EVENT_QUIT)
			*running = 0;
		else if (event.type == MQT_EVENT_KEY_DOWN && event.key_code == 27)
			*running = 0;
		else if (event.type == MQT_EVENT_MOUSE_MOVE)
		{
			*mouse_x = event.mouse_x;
			*mouse_y = event.mouse_y;
		}
		else if (event.type == MQT_EVENT_MOUSE_DOWN)
		{
			*mouse_x = event.mouse_x;
			*mouse_y = event.mouse_y;
			*mouse_down = 1;
			for (int i = 0; i < 4; i++)
				mqt_button_handle_click(buttons[i], *mouse_x, *mouse_y);
		}
		else if (event.type == MQT_EVENT_MOUSE_UP)
			*mouse_down = 0;
	}
}

int	main(void)
{
	t_backend_context	*ctx;
	t_button			**buttons;
	int					vars[4];

	printf("[Demo] Iniciando widget demo...\n");
	ctx = mqt_create_auto_backend(800, 600, "MiniQT Widget Demo");
	if (!ctx)
		return (1);
	buttons = create_buttons();
	if (!buttons)
	{
		mqt_backend_destroy(ctx);
		return (1);
	}
	vars[0] = 1;
	vars[1] = 0;
	vars[2] = 0;
	vars[3] = 0;
	printf("[Demo] Loop principal iniciado. Use o mouse!\n");
	while (vars[0])
	{
		handle_events(ctx, buttons, &vars[0], &vars[1], &vars[2], &vars[3]);
		for (int i = 0; i < 4; i++)
			mqt_button_update(buttons[i], vars[1], vars[2], vars[3]);
		draw_ui(ctx, buttons, 4);
		if (ctx->backend_type == MQT_BACKEND_TERMINAL)
		{
			sleep(3);
			vars[0] = 0;
		}
		else
			usleep(16666);
	}
	for (int i = 0; i < 4; i++)
		mqt_button_destroy(buttons[i]);
	free(buttons);
	mqt_backend_destroy(ctx);
	printf("[Demo] Finalizado com sucesso!\n");
	return (0);
}
