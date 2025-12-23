/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_draw.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MQT_DRAW_H
# define MQT_DRAW_H

# include "mqt_framebuffer.h"
# include "../miniqt_backend/backend.h"

/* Funções de desenho básicas - usam t_rect de backend.h */
void	mqt_draw_rect(t_framebuffer *fb, t_rect rect, t_pixel pixel);
void	mqt_draw_rect_filled(t_framebuffer *fb, t_rect rect, t_pixel pixel);
void	mqt_draw_line_h(t_framebuffer *fb, int x, int y, int length,
			t_pixel pixel);
void	mqt_draw_line_v(t_framebuffer *fb, int x, int y, int length,
			t_pixel pixel);
void	mqt_draw_text(t_framebuffer *fb, int x, int y, const char *text,
			t_term_color fg, t_term_color bg);
void	mqt_draw_border(t_framebuffer *fb, t_rect rect, t_term_color fg,
			t_term_color bg);

/* Utilitários */
t_rect	mqt_rect(int x, int y, int width, int height);

#endif
