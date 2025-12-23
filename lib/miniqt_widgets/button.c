/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "button.h"
#include <stdlib.h>
#include <string.h>

t_button	*mqt_button_create(const char *label, int x, int y,
		int width, int height)
{
	t_button	*btn;

	btn = malloc(sizeof(t_button));
	if (!btn)
		return (NULL);
	btn->label = strdup(label ? label : "Button");
	if (!btn->label)
	{
		free(btn);
		return (NULL);
	}
	btn->bounds = mqt_make_rect(x, y, width, height);
	btn->state = BUTTON_NORMAL;
	btn->is_enabled = 1;
	btn->normal_color = mqt_color(80, 120, 200, 255);
	btn->hover_color = mqt_color(100, 140, 220, 255);
	btn->pressed_color = mqt_color(60, 100, 180, 255);
	btn->disabled_color = mqt_color(150, 150, 150, 255);
	btn->text_color = mqt_color(255, 255, 255, 255);
	btn->user_data = NULL;
	btn->on_click = NULL;
	return (btn);
}

void	mqt_button_destroy(t_button *btn)
{
	if (!btn)
		return ;
	if (btn->label)
		free(btn->label);
	free(btn);
}

void	mqt_button_draw(t_backend_context *ctx, t_button *btn)
{
	t_color	bg_color;
	int		text_width;
	int		text_x;
	int		text_y;

	if (!btn)
		return ;
	if (btn->state == BUTTON_HOVER)
		bg_color = btn->hover_color;
	else if (btn->state == BUTTON_PRESSED)
		bg_color = btn->pressed_color;
	else if (btn->state == BUTTON_DISABLED)
		bg_color = btn->disabled_color;
	else
		bg_color = btn->normal_color;
	mqt_draw_rectangle_filled(ctx, btn->bounds, bg_color);
	mqt_draw_rectangle(ctx, btn->bounds, mqt_color(100, 100, 100, 255));
	if (btn->label)
	{
		text_width = strlen(btn->label) * 8;
		text_x = btn->bounds.x + (btn->bounds.width - text_width) / 2;
		text_y = btn->bounds.y + (btn->bounds.height - 14) / 2;
		mqt_draw_string(ctx, text_x, text_y, btn->label, btn->text_color);
	}
}

int	mqt_button_is_hovered(t_button *btn, int mouse_x, int mouse_y)
{
	return (mouse_x >= btn->bounds.x
		&& mouse_x <= btn->bounds.x + btn->bounds.width
		&& mouse_y >= btn->bounds.y
		&& mouse_y <= btn->bounds.y + btn->bounds.height);
}

void	mqt_button_update(t_button *btn, int mouse_x, int mouse_y,
		int mouse_down)
{
	int	hovered;

	if (!btn->is_enabled)
	{
		btn->state = BUTTON_DISABLED;
		return ;
	}
	hovered = mqt_button_is_hovered(btn, mouse_x, mouse_y);
	if (hovered && mouse_down)
		btn->state = BUTTON_PRESSED;
	else if (hovered)
		btn->state = BUTTON_HOVER;
	else
		btn->state = BUTTON_NORMAL;
}

int	mqt_button_handle_click(t_button *btn, int mouse_x, int mouse_y)
{
	if (!btn->is_enabled)
		return (0);
	if (mqt_button_is_hovered(btn, mouse_x, mouse_y))
	{
		if (btn->on_click)
			btn->on_click(btn);
		return (1);
	}
	return (0);
}

void	mqt_button_set_enabled(t_button *btn, int enabled)
{
	if (btn)
		btn->is_enabled = enabled;
}

void	mqt_button_set_label(t_button *btn, const char *label)
{
	char	*new_label;

	if (!btn || !label)
		return ;
	new_label = strdup(label);
	if (!new_label)
		return ;
	if (btn->label)
		free(btn->label);
	btn->label = new_label;
}

void	mqt_button_set_colors(t_button *btn, t_color normal, t_color hover,
		t_color pressed)
{
	if (!btn)
		return ;
	btn->normal_color = normal;
	btn->hover_color = hover;
	btn->pressed_color = pressed;
}
