/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:16:52 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/12 17:43:19 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_gnl_node	*alloc_remainder(t_gnl_node *tail, int i)
{
	t_gnl_node	*rem_node;
	int			j;

	rem_node = malloc(sizeof(t_gnl_node));
	if (!rem_node)
		return (NULL);
	rem_node->next = NULL;
	j = 0;	
	while (tail->content[i + j])
		j++;
	rem_node->content = malloc(sizeof(char) * (j + 1));
	if (!rem_node->content)
	{
		free(rem_node);
		return (NULL);
	}
	j = 0;
	while (tail->content[i])
		rem_node->content[j++] = tail->content[i++];
	rem_node->content[j] = '\0';
	return (rem_node);
}

void	update_list(t_gnl_list *list)
{
	t_gnl_node	*rem_node;
	int			i;

	if (!list || !list->tail)
		return ;
	i = 0;
	while (list->tail->content[i] && list->tail->content[i] != '\n')
		i++;
	if (list->tail->content[i] == '\n')
		i++;
	rem_node = alloc_remainder(list->tail, i);
	free_list(list);
	if (rem_node && rem_node->content[0] != '\0')
	{
		list->head = rem_node;
		list->tail = rem_node;
	}
	else if (rem_node)
	{
		free(rem_node->content);
		free(rem_node);
	}
}

char	*extract_line(t_gnl_list *list)
{
	char	*line;
	int		len;

	if (!list || !list->head)
		return (NULL);
	len = count_len(list);
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	copy_line(list, line);
	return (line);
}

void	read_and_append(int fd, t_gnl_list *list)
{
	char	*buffer;
	int		bytes_read;

	while (!found_newline(list))
	{
		buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
			return ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(buffer);
			if (bytes_read < 0)
				free_list(list);
			return ;
		}
		buffer[bytes_read] = '\0';
		add_to_list(list, buffer);
		if (!list->head)
			return ;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl_list	list;
	char				*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	read_and_append(fd, &list);
	if (!list.head)
		return (NULL);
	line = extract_line(&list);
	update_list(&list);
	return (line);
}
