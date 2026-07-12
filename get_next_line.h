/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 12:19:10 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/11 23:20:06 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>

typedef struct s_gnl_node
{
	char				*content;
	struct s_gnl_node	*next;
}	t_gnl_node;

typedef struct s_gnl_list
{
	t_gnl_node	*head;
	t_gnl_node	*tail;
}	t_gnl_list;

char	*get_next_line(int fd);
void	read_and_append(int fd, t_gnl_list *list);
void	add_to_list(t_gnl_list *list, char *buf);
char	*extract_line(t_gnl_list *list);
void	update_list(t_gnl_list *list);
int		found_newline(t_gnl_list *list);
void	free_list(t_gnl_list *list);
int		count_len(t_gnl_list *list);
void	copy_line(t_gnl_list *list, char *line);

#endif