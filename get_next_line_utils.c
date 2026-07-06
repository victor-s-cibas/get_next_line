/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:17:58 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/05 21:53:09 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	found_newline(t_gnl_list *list)
{
	int	i;

	if (!list)
		return (0);
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			if (list->content[i] == '\n')
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}

void	free_list(t_gnl_list *list)
{
	t_gnl_list	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->content);
		free(list);
		list = tmp;
	}
}

int	count_len(t_gnl_list *list)
{
	int	len;
	int	i;

	len = 0;
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			len++;
			if (list->content[i] == '\n')
				return (len);
			i++;
		}
		list = list->next;
	}
	return (len);
}

void	copy_line(t_gnl_list *list, char *line)
{
	int	i;
	int	j;

	if (!list)
		return ;
	j = 0;
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			line[j++] = list->content[i];
			if (list->content[i] == '\n')
			{
				line[j] = '\0';
				return ;
			}
			i++;
		}
		list = list->next;
	}
	line[j] = '\0';
}

void	add_to_list(t_gnl_list **list, char *buf)
{
	t_gnl_list	*new_node;
	t_gnl_list	*last_node;

	new_node = malloc(sizeof(t_gnl_list));
	if (!new_node)
	{
		free(buf);
		free_list(*list);
		*list = NULL;
		return ;
	}
	new_node->content = buf;
	new_node->next = NULL;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	last_node = *list;
	while (last_node->next)
		last_node = last_node->next;
	last_node->next = new_node;
}
