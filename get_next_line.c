/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:16:52 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/05 22:54:36 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_gnl_list	*alloc_remainder(t_gnl_list *last_node, int i)
{
	t_gnl_list	*rem_node;
	int			j;

	rem_node = malloc(sizeof(t_gnl_list));
	if (!rem_node)
		return (NULL);
	rem_node->next = NULL;
	j = 0;
	while (last_node->content[i + j])
		j++;
	rem_node->content = malloc(sizeof(char) * (j + 1));
	if (!rem_node->content)
	{
		free(rem_node);
		return (NULL);
	}
	j = 0;
	while (last_node->content[i])
		rem_node->content[j++] = last_node->content[i++];
	rem_node->content[j] = '\0';
	return (rem_node);
}

void	update_list(t_gnl_list **list)
{
	t_gnl_list	*last_node;
	t_gnl_list	*rem_node;
	int			i;

	if (!list || !*list)
		return ;
	last_node = *list;
	while (last_node->next)
		last_node = last_node->next;
	i = 0;
	while (last_node->content[i] && last_node->content[i] != '\n')
		i++;
	if (last_node->content[i] == '\n')
		i++;
	rem_node = alloc_remainder(last_node, i);
	free_list(*list);
	*list = rem_node;
	if (*list && (*list)->content[0] == '\0')
	{
		free_list(*list);
		*list = NULL;
	}
}

char	*extract_line(t_gnl_list *list)
{
	char	*line;
	int		len;

	if (!list)
		return (NULL);
	len = count_len(list);
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	copy_line(list, line);
	return (line);
}

void	read_and_append(int fd, t_gnl_list **list)
{
	char	*buffer;
	int		bytes_read;

	while (!found_newline(*list))
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
			return ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(buffer);
			if (bytes_read < 0)
			{
				free_list(*list);
				*list = NULL;
			}
			return ;
		}
		buffer[bytes_read] = '\0';
		add_to_list(list, buffer);
		if (!*list)
			return ;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl_list	*list;
	char				*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_and_append(fd, &list);
	if (!list)
		return (NULL);
	line = extract_line(list);
	update_list(&list);
	return (line);
}
