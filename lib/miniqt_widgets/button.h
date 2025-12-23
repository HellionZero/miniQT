/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_H
# define BUTTON_H

# include "../miniqt_backend/backend.h"

/* Estados do botão */
typedef enum e_button_state
{
	BUTTON_NORMAL,
	BUTTON_HOVER,
	BUTTON_PRESSED,
	BUTTON_DISABLED
}	t_button_state;

/* Estrutura do botão */
typedef struct s_button
{
	char			*label;
	t_rect			bounds;
	t_button_state	state;
	int				is_enabled;
	t_color			normal_color;
	t_color			hover_color;
	t_color			pressed_color;
	t_color			disabled_color;
	t_color			text_color;
	void			*user_data;
	void			(*on_click)(struct s_button *btn);
}	t_button;

/* Gerenciamento de botões */
t_button	*mqt_button_create(const char *label, int x, int y,
				int width, int height);
void		mqt_button_destroy(t_button *btn);

/* Renderização */
void		mqt_button_draw(t_backend_context *ctx, t_button *btn);

/* Interação */
void		mqt_button_update(t_button *btn, int mouse_x, int mouse_y,
				int mouse_down);
int			mqt_button_is_hovered(t_button *btn, int mouse_x, int mouse_y);
int			mqt_button_handle_click(t_button *btn, int mouse_x, int mouse_y);

/* Propriedades */
void		mqt_button_set_enabled(t_button *btn, int enabled);
void		mqt_button_set_label(t_button *btn, const char *label);
void		mqt_button_set_colors(t_button *btn, t_color normal, t_color hover,
				t_color pressed);

#endif
