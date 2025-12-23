/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mqt_framebuffer.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MQT_FRAMEBUFFER_H
# define MQT_FRAMEBUFFER_H

# include "mqt_terminal.h"
# include <stdlib.h>

/* Pixel com cor e caractere */
typedef struct s_pixel
{
	char			character;
	t_term_color	fg_color;
	t_term_color	bg_color;
}	t_pixel;

/* Framebuffer - representação da tela em memória */
typedef struct s_framebuffer
{
	int		width;
	int		height;
	t_pixel	**pixels;
}	t_framebuffer;

/* Gerenciamento do framebuffer */
t_framebuffer	*mqt_fb_create(int width, int height);
void			mqt_fb_destroy(t_framebuffer *fb);
void			mqt_fb_clear(t_framebuffer *fb, t_term_color bg_color);
void			mqt_fb_render(t_framebuffer *fb);

/* Desenho de pixels */
void			mqt_fb_set_pixel(t_framebuffer *fb, int x, int y,
					t_pixel pixel);
t_pixel			mqt_fb_get_pixel(t_framebuffer *fb, int x, int y);
t_pixel			mqt_pixel(char c, t_term_color fg, t_term_color bg);

#endif
