/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alen <alen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:47:13 by alen              #+#    #+#             */
/*   Updated: 2025/03/17 17:23:33 by alen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>

void	ft_free_split(char **str);
void	print_error(char *str, int code);

typedef struct s_pipex
{
	int		fd[2];
	int		open_fd;
	pid_t	pid;
	pid_t	pid1;
	char	*tmp;
	char	*full_path;
	char	**args;
}	t_pipex;

#endif
