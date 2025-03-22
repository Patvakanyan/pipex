/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alen <alen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:25:29 by alen              #+#    #+#             */
/*   Updated: 2025/03/21 14:49:54 by alen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		++i;
	}
	free (str);
}

void	print_error(char *str, int code)
{
	ft_putstr_fd(str, 2);
	exit(code);
}
