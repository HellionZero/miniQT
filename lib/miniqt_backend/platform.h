/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   platform.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLATFORM_H
# define PLATFORM_H

/* Detecção de Sistema Operacional */
# if defined(__linux__)
#  define MQT_PLATFORM_LINUX 1
#  define MQT_PLATFORM_NAME "Linux"
# elif defined(__APPLE__) && defined(__MACH__)
#  define MQT_PLATFORM_MACOS 1
#  define MQT_PLATFORM_NAME "macOS"
# elif defined(_WIN32) || defined(_WIN64)
#  define MQT_PLATFORM_WINDOWS 1
#  define MQT_PLATFORM_NAME "Windows"
# elif defined(__FreeBSD__)
#  define MQT_PLATFORM_FREEBSD 1
#  define MQT_PLATFORM_NAME "FreeBSD"
# else
#  define MQT_PLATFORM_UNKNOWN 1
#  define MQT_PLATFORM_NAME "Unknown"
# endif

/* Tipo de Backend Gráfico Disponível */
typedef enum e_graphics_backend
{
	MQT_BACKEND_NONE = 0,
	MQT_BACKEND_TERMINAL,
	MQT_BACKEND_SDL2,
	MQT_BACKEND_X11,
	MQT_BACKEND_WAYLAND,
	MQT_BACKEND_WIN32,
	MQT_BACKEND_COCOA
}	t_graphics_backend;

/* Informações da Plataforma */
typedef struct s_platform_info
{
	const char			*os_name;
	t_graphics_backend	preferred_backend;
	int					has_x11;
	int					has_wayland;
	int					has_display;
	int					is_headless;
	int					is_ssh_session;
}	t_platform_info;

/* Funções de Detecção */
t_platform_info		mqt_detect_platform(void);
t_graphics_backend	mqt_get_best_backend(void);
const char			*mqt_backend_name(t_graphics_backend backend);
int					mqt_backend_available(t_graphics_backend backend);

#endif
