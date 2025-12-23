/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   platform.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	check_display_available(void)
{
	char	*display;
	char	*wayland_display;

	display = getenv("DISPLAY");
	wayland_display = getenv("WAYLAND_DISPLAY");
	if (display || wayland_display)
		return (1);
	return (0);
}

static int	check_x11_available(void)
{
	char	*display;

	display = getenv("DISPLAY");
	if (display && *display)
		return (1);
	return (0);
}

static int	check_wayland_available(void)
{
	char	*wayland;

	wayland = getenv("WAYLAND_DISPLAY");
	if (wayland && *wayland)
		return (1);
	return (0);
}

t_platform_info	mqt_detect_platform(void)
{
	t_platform_info	info;

	info.os_name = MQT_PLATFORM_NAME;
	info.has_x11 = check_x11_available();
	info.has_wayland = check_wayland_available();
	info.has_display = check_display_available();
	info.is_headless = !info.has_display;

	#ifdef MQT_PLATFORM_LINUX
	if (info.has_wayland)
		info.preferred_backend = MQT_BACKEND_SDL2;
	else if (info.has_x11)
		info.preferred_backend = MQT_BACKEND_SDL2;
	else
		info.preferred_backend = MQT_BACKEND_TERMINAL;
	#elif defined(MQT_PLATFORM_MACOS)
	info.preferred_backend = MQT_BACKEND_SDL2;
	#elif defined(MQT_PLATFORM_WINDOWS)
	info.preferred_backend = MQT_BACKEND_SDL2;
	#else
	info.preferred_backend = MQT_BACKEND_TERMINAL;
	#endif

	return (info);
}

t_graphics_backend	mqt_get_best_backend(void)
{
	t_platform_info	info;

	info = mqt_detect_platform();
	return (info.preferred_backend);
}

const char	*mqt_backend_name(t_graphics_backend backend)
{
	if (backend == MQT_BACKEND_TERMINAL)
		return ("Terminal");
	else if (backend == MQT_BACKEND_SDL2)
		return ("SDL2");
	else if (backend == MQT_BACKEND_X11)
		return ("X11");
	else if (backend == MQT_BACKEND_WAYLAND)
		return ("Wayland");
	else if (backend == MQT_BACKEND_WIN32)
		return ("Win32");
	else if (backend == MQT_BACKEND_COCOA)
		return ("Cocoa");
	return ("None");
}

int	mqt_backend_available(t_graphics_backend backend)
{
	t_platform_info	info;

	info = mqt_detect_platform();
	if (backend == MQT_BACKEND_TERMINAL)
		return (1);
	else if (backend == MQT_BACKEND_SDL2)
		return (info.has_display);
	else if (backend == MQT_BACKEND_X11)
		return (info.has_x11);
	else if (backend == MQT_BACKEND_WAYLAND)
		return (info.has_wayland);
	return (0);
}
