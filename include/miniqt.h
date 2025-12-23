/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniqt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 15:29:47 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 15:43:51 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIQT_H
# define MINIQT_H

/*
 * MiniQT - Framework Gráfico Multi-Plataforma
 * 
 * API auxiliar para criação de interfaces gráficas em C puro.
 * Suporta múltiplos backends (Terminal ANSI, SDL2, etc.)
 * 
 * Para usar em outros programas:
 *   1. Incluir este header: #include "miniqt.h"
 *   2. Linkar com: -lminiqt -lft -lftprintf
 *   3. (Opcional) Compilar com SDL2: -DMQT_USE_SDL2 -lSDL2
 * 
 * Exemplo básico:
 *   t_backend_context *ctx = mqt_create_auto_backend(800, 600, "Título");
 *   mqt_begin_frame(ctx);
 *   mqt_draw_rectangle_filled(ctx, mqt_make_rect(10, 10, 100, 50), 
 *                              mqt_color(255, 0, 0, 255));
 *   mqt_end_frame(ctx);
 *   mqt_backend_destroy(ctx);
 */

/* Bibliotecas base */
# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"

/* Sistema de backends (API principal) */
# include "../lib/miniqt_backend/platform.h"
# include "../lib/miniqt_backend/backend.h"
# include "../lib/miniqt_backend/backend_factory.h"

/* Primitivas de renderização (nível baixo - opcional) */
# include "../lib/miniqt_render/mqt_terminal.h"
# include "../lib/miniqt_render/mqt_framebuffer.h"
# include "../lib/miniqt_render/mqt_draw.h"

# include <unistd.h>

#endif