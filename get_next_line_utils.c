/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:17:58 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/12 00:23:07 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	found_newline(t_gnl_list *list)
{
	int	i;

	if (!list || !list->tail)
		return (0);
	i = 0;
	while (list->tail->content[i])
	{
		if (list->tail->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

void	free_list(t_gnl_list *list)
{
	t_gnl_node	*tmp;
	t_gnl_node	*current;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		tmp = current->next;
		free(current->content);
		free(current);
		current = tmp;
	}
	list->head = NULL;
	list->tail = NULL;
}

int	count_len(t_gnl_list *list)
{
	int			len;
	int			i;
	t_gnl_node	*current;

	len = 0;
	current = list->head;
	while (current)
	{
		i = 0;
		while (current->content[i])
		{
			len++;
			if (current->content[i++] == '\n')
				return (len);
		}
		current = current->next;
	}
	return (len);
}

void	copy_line(t_gnl_list *list, char *line)
{
	int			i;
	int			j;
	t_gnl_node	*current;

	j = 0;
	current = list->head;
	while (current)
	{
		i = 0;
		while (current->content[i])
		{
			line[j++] = current->content[i];
			if (current->content[i] == '\n')
			{
				line[j] = '\0';
				return ;
			}
			i++;
		}
		current = current->next;
	}
	line[j] = '\0';
}

void	add_to_list(t_gnl_list *list, char *buf)
{
	t_gnl_node	*new_node;

	new_node = malloc(sizeof(t_gnl_node));
	if (!new_node)
	{
		free(buf);
		free_list(list);
		return ;
	}
	new_node->content = buf;
	new_node->next = NULL;
	if (!list->head)
		list->head = new_node;
	else
		list->tail->next = new_node;
	list->tail = new_node;
}
