/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:16:52 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/02 18:19:16 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


int read_line(char *local_buffer, t_gnl_list **stash, int fd)
{
		int	bytes_total;

		bytes_total = 1;
		while (bytes_total > 0 && !no_new_line(*stash))
	{
		bytes_total = read(fd, local_buffer, BUFFER_SIZE);
		if (bytes_total < 0)
		{
			ft_lstclear(stash);
			free(local_buffer);
			return  (0);
		}
		if (bytes_total == 0)
			return (1);
		local_buffer[bytes_total] = '\0';
		add_to_stash(stash, local_buffer);
	}
	return (1);
}

char *get_next_line(int fd)
{
	static t_gnl_list	*stash;
	char				*end_line;
	char				*local_buffer;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	
	local_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!local_buffer)
		return (NULL);
	if (!read_line(local_buffer, &stash, fd))
		return (NULL);
	read_line(local_buffer, &stash, fd);
	free(local_buffer);
	end_line = extract_line_from_list(stash);
	update_stash(&stash);
	return(end_line);
}
