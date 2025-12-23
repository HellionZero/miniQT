/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backend_factory.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by lsarraci          #+#    #+#             */
/*   Updated: 2025/12/23 00:00:00 by lsarraci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKEND_FACTORY_H
# define BACKEND_FACTORY_H

# include "backend.h"

/*
 * Factory Functions
 */

/* Cria backend automaticamente baseado no ambiente */
t_backend_context	*mqt_create_auto_backend(int width, int height,
						const char *title);

/* Cria backend específico */
t_backend_context	*mqt_create_backend(t_graphics_backend backend_type,
						int width, int height, const char *title);

/* Lista backends disponíveis */
void				mqt_list_available_backends(void);

#endif
