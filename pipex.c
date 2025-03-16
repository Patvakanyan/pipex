/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alen <alen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:43:20 by alen              #+#    #+#             */
/*   Updated: 2025/03/16 00:11:11 by alen             ###   ########.fr       */
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

char	*path_acsses(char **envp, char **args)
{
	char	*full_path;
	char	*tmp;
	int		i;
	char	*clean_path;
	char	**split_path;

	i = 0;
	full_path = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			full_path = envp[i] + 5;
			break ;
		}
		++i;
	}
	split_path = ft_split(full_path, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i])
	{
		tmp = ft_strjoin(split_path[i], "/");
		clean_path = ft_strjoin(tmp, args[0]);
		free(tmp);
		if (access(clean_path, X_OK) == 0)
		{
			ft_free_split(split_path);
			return (clean_path);
		}
		free(clean_path);
		i++;
	}
	ft_free_split(split_path);
	return (NULL);
}

void	child_process(int *fd, char **envp, char **argv)
{
	char	*full_path;
	char	**args;
	int		open_fd;

	close(fd[0]);
	open_fd = open(argv[1], O_RDONLY);
	if (open_fd < 0)
	{
		perror("Can't open file");
		exit(1);
	}
	args = ft_split(argv[2], ' ');
	if (!args)
	{
		perror("split");
		exit(1);
	}
	full_path = path_acsses(envp, args);
	dup2(open_fd, 0);
	if (dup2(fd[1], 1) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(open_fd);
	close(fd[1]);
	if (execve(full_path, args, envp) == -1)
	{
		perror("execve");
		exit(1);
	}
	ft_free_split(args);
}

void	child_one_process(int *fd, char **envp, char **argv)
{
	char	*full_path;
	char	**args;
	int		open_fd;

	close(fd[1]);
	open_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (open_fd < 0)
	{
		perror("Can't open file");
		exit(1);
	}
	args = ft_split(argv[3], ' ');
	if (!args)
	{
		perror("one split");
		exit(1);
	}
	full_path = path_acsses(envp, args);
	dup2(fd[0], 0);
	if (dup2(open_fd, 1) < 0)
	{
		perror("dup2");
		exit(1);
	}
	close(open_fd);
	close(fd[0]);
	if (execve(full_path, args, envp) == -1)
	{
		perror("execve");
		exit(1);
	}
	ft_free_split(args);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid1;

	if (argc != 5)
	{
		write(2, "Cannot create pipe.", 19);
		exit(1);
	}
	if (pipe(fd) < 0)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == 0)
		child_process(fd, envp, argv);
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid1 == 0)
		child_one_process(fd, envp, argv);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid1, NULL, 0);
}
