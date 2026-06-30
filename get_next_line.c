/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:16:52 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/06/30 17:18:11 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
// lembrar de remover todos os includes da lib depois de
// refazer as funcoes

char *get_next_line(int fd)
{
	char c;
	static char str[35];
	char *ptr;

	ptr = str;

	while (read(fd, &c, 1) > 0)
	{
		*ptr = c;
		ptr++;
	}
	*ptr = '\0';
	
	return(str);
}