/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend_factory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Factory que cria o backend apropriado baseado no ambiente
 * Ordem de preferência:
 * 1. SDL2 (se display disponível e SDL2 compilado)
 * 2. Terminal (fallback sempre disponível)
 */
t_backend_context	*mqt_create_auto_backend(int width, int height,
		const char *title)
{
	t_graphics_backend	backend;
	t_backend_context	*ctx;
	t_platform_info		platform;

	platform = mqt_detect_platform();
	backend = platform.preferred_backend;
	if (!mqt_backend_available(backend))
		backend = MQT_BACKEND_TERMINAL;
	printf("[MiniQT] Platform: %s\n", platform.os_name);
	if (platform.is_ssh_session)
		printf("[MiniQT] SSH session detected\n");
	printf("[MiniQT] Display available: %s\n", platform.has_display
		? "yes" : "no");
	if (platform.has_x11)
		printf("[MiniQT] X11 forwarding: active\n");
	printf("[MiniQT] Using backend: %s\n", mqt_backend_name(backend));
	ctx = mqt_backend_create(backend, width, height, title);
	if (!ctx)
	{
		printf("[MiniQT] Failed to create %s backend, falling back to "
			"terminal\n", mqt_backend_name(backend));
		ctx = mqt_backend_create(MQT_BACKEND_TERMINAL, width, height, title);
	}
	return (ctx);
}

/*
 * Cria backend específico (útil para forçar um backend)
 */
t_backend_context	*mqt_create_backend(t_graphics_backend backend_type,
		int width, int height, const char *title)
{
	if (!mqt_backend_available(backend_type))
	{
		printf("[MiniQT] Backend %s not available\n",
			mqt_backend_name(backend_type));
		return (NULL);
	}
	return (mqt_backend_create(backend_type, width, height, title));
}

/*
 * Lista todos os backends disponíveis no sistema atual
 */
void	mqt_list_available_backends(void)
{
	t_platform_info		platform;
	t_graphics_backend	backends[] = {
		MQT_BACKEND_TERMINAL,
		MQT_BACKEND_SDL2,
		MQT_BACKEND_X11,
		MQT_BACKEND_WAYLAND,
		MQT_BACKEND_WIN32,
		MQT_BACKEND_COCOA
	};
	int					i;

	platform = mqt_detect_platform();
	printf("[MiniQT] Platform: %s\n", platform.os_name);
	printf("[MiniQT] Available backends:\n");
	i = 0;
	while (i < 6)
	{
		if (mqt_backend_available(backends[i]))
		{
			printf("  - %s", mqt_backend_name(backends[i]));
			if (backends[i] == platform.preferred_backend)
				printf(" (preferred)");
			printf("\n");
		}
		i++;
	}
}
