/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKEND_H
# define BACKEND_H

/*
 * ⚠️  THREAD SAFETY WARNING:
 * 
 * Esta API NÃO é thread-safe. Todas as operações de renderização
 * devem ocorrer na mesma thread (preferencialmente a thread principal).
 * 
 * SDL2 requer que operações de janela/renderização sejam executadas
 * na thread que criou o contexto (geralmente a thread principal).
 * 
 * Uso incorreto (múltiplas threads):
 *   ❌ pthread_create(..., render_thread1, ctx);  // PERIGOSO
 *   ❌ pthread_create(..., render_thread2, ctx);  // PERIGOSO
 * 
 * Uso correto (single thread):
 *   ✅ Toda renderização no main() ou em uma única thread dedicada
 *   ✅ Eventos podem ser processados em thread separada, mas
 *      desenho deve ser sincronizado com a thread de renderização
 */

# include "platform.h"
# include "../miniqt_render/mqt_terminal.h"

/* Forward declarations */
typedef struct s_backend_context	t_backend_context;
typedef struct s_backend_ops		t_backend_ops;

/* Estrutura de Cor Genérica */
typedef struct s_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_color;

/* Estrutura de Retângulo */
typedef struct s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
}	t_rect;

/* Eventos do Sistema */
typedef enum e_event_type
{
	MQT_EVENT_NONE = 0,
	MQT_EVENT_QUIT,
	MQT_EVENT_KEY_DOWN,
	MQT_EVENT_KEY_UP,
	MQT_EVENT_MOUSE_DOWN,
	MQT_EVENT_MOUSE_UP,
	MQT_EVENT_MOUSE_MOVE,
	MQT_EVENT_WINDOW_RESIZE
}	t_event_type;

typedef struct s_event
{
	t_event_type	type;
	int				key_code;
	int				mouse_x;
	int				mouse_y;
	int				mouse_button;
}	t_event;

/* Interface de Operações do Backend (Virtual Table) */
struct s_backend_ops
{
	/* Inicialização e destruição */
	int		(*init)(t_backend_context *ctx, int width, int height,
				const char *title);
	void	(*destroy)(t_backend_context *ctx);

	/* Controle de renderização */
	void	(*begin_frame)(t_backend_context *ctx);
	void	(*end_frame)(t_backend_context *ctx);
	void	(*clear)(t_backend_context *ctx, t_color color);

	/* Desenho primitivo */
	void	(*draw_rect)(t_backend_context *ctx, t_rect rect, t_color color);
	void	(*draw_rect_filled)(t_backend_context *ctx, t_rect rect,
				t_color color);
	void	(*draw_text)(t_backend_context *ctx, int x, int y,
				const char *text, t_color color);

	/* Gerenciamento de eventos */
	int		(*poll_event)(t_backend_context *ctx, t_event *event);

	/* Utilitários */
	void	(*set_title)(t_backend_context *ctx, const char *title);
	void	(*get_size)(t_backend_context *ctx, int *width, int *height);
};

/* Contexto do Backend (Estado) */
struct s_backend_context
{
	t_graphics_backend	backend_type;
	const t_backend_ops	*ops;
	void				*backend_data;
	int					width;
	int					height;
	char				*title;
	int					is_running;
};

/* Funções Públicas da API */
t_backend_context	*mqt_backend_create(t_graphics_backend backend_type,
						int width, int height, const char *title);
void				mqt_backend_destroy(t_backend_context *ctx);

/* Funções inline para chamar operações */
static inline void	mqt_begin_frame(t_backend_context *ctx)
{
	if (ctx && ctx->ops && ctx->ops->begin_frame)
		ctx->ops->begin_frame(ctx);
}

static inline void	mqt_end_frame(t_backend_context *ctx)
{
	if (ctx && ctx->ops && ctx->ops->end_frame)
		ctx->ops->end_frame(ctx);
}

static inline void	mqt_clear_screen_color(t_backend_context *ctx, t_color c)
{
	if (ctx && ctx->ops && ctx->ops->clear)
		ctx->ops->clear(ctx, c);
}

static inline void	mqt_draw_rectangle(t_backend_context *ctx, t_rect r,
	t_color c)
{
	if (ctx && ctx->ops && ctx->ops->draw_rect)
		ctx->ops->draw_rect(ctx, r, c);
}

static inline void	mqt_draw_rectangle_filled(t_backend_context *ctx,
	t_rect r, t_color c)
{
	if (ctx && ctx->ops && ctx->ops->draw_rect_filled)
		ctx->ops->draw_rect_filled(ctx, r, c);
}

static inline void	mqt_draw_string(t_backend_context *ctx, int x, int y,
	const char *text, t_color c)
{
	if (ctx && ctx->ops && ctx->ops->draw_text)
		ctx->ops->draw_text(ctx, x, y, text, c);
}

static inline int	mqt_poll_event_ctx(t_backend_context *ctx, t_event *event)
{
	if (ctx && ctx->ops && ctx->ops->poll_event)
		return (ctx->ops->poll_event(ctx, event));
	return (0);
}

/* Utilitários */
t_color	mqt_color(unsigned char r, unsigned char g, unsigned char b,
			unsigned char a);
t_rect	mqt_make_rect(int x, int y, int width, int height);

#endif
