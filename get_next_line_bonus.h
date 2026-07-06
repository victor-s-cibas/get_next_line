/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicdos-s <vicdos-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:18:11 by vicdos-s          #+#    #+#             */
/*   Updated: 2026/07/06 15:53:15 by vicdos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

typedef struct s_gnl_list
{
	char				*content;
	struct s_gnl_list	*next;
}	t_gnl_list;

typedef struct s_gnl_context
{
	int					fd;
	struct s_gnl_list	*list;
} t_gnl_context;

#endif