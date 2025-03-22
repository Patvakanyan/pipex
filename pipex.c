/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alen <alen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:43:20 by alen              #+#    #+#             */
/*   Updated: 2025/03/22 18:27:46 by alen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*search_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		++i;
	}
	return (NULL);
}

char	*path_acsses(char **envp, char **args, t_pipex pipex)
{
	int		i;
	char	*clean_path;
	char	**split_path;

	if (ft_strchr(args[0], '/') != NULL && access(args[0], X_OK) == 0)
		return (args[0]);
	i = 0;
	pipex.full_path = search_path(envp);
	split_path = ft_split(pipex.full_path, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i])
	{
		pipex.tmp = ft_strjoin(split_path[i], "/");
		clean_path = ft_strjoin(pipex.tmp, args[0]);
		free(pipex.tmp);
		if (access(clean_path, X_OK) == 0)
			return (ft_free_split(split_path), clean_path);
		free(clean_path);
		i++;
	}
	ft_free_split(split_path);
	return (NULL);
}

void	child_process(int *fd, char **envp, char **argv, t_pipex pipex)
{
	close(fd[0]);
	pipex.open_fd = open(argv[1], O_RDONLY);
	if (pipex.open_fd < 0)
		print_error("Can't open file!", EXIT_FAILURE);
	pipex.args = ft_split(argv[2], ' ');
	if (!(pipex.args))
		print_error("split failed!", EXIT_FAILURE);
	pipex.full_path = path_acsses(envp, pipex.args, pipex);
	if (!pipex.full_path)
		print_error("Command not found\n", 127);
	if (dup2(pipex.open_fd, 0) < 0)
		print_error("dup2 failed!", EXIT_FAILURE);
	if (dup2(fd[1], 1) < 0)
		print_error("dup2 failed!", EXIT_FAILURE);
	close(pipex.open_fd);
	close(fd[1]);
	if (execve(pipex.full_path, pipex.args, envp) == -1)
		print_error("execve failed!", EXIT_FAILURE);
	ft_free_split(pipex.args);
}

void	child_one_process(int *fd, char **envp, char **argv, t_pipex pipex)
{
	close(fd[1]);
	pipex.open_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.open_fd < 0)
		print_error("Can't open file!", EXIT_FAILURE);
	pipex.args = ft_split(argv[3], ' ');
	if (!(pipex.args))
		print_error("split failed!", EXIT_FAILURE);
	pipex.full_path = path_acsses(envp, pipex.args, pipex);
	if (!pipex.full_path)
		print_error("Command not found\n", 127);
	dup2(fd[0], 0);
	if (dup2(pipex.open_fd, 1) < 0)
		print_error("execve failed!", EXIT_FAILURE);
	close(pipex.open_fd);
	close(fd[0]);
	if (execve(pipex.full_path, pipex.args, envp) == -1)
		print_error("execve failed!", EXIT_FAILURE);
	ft_free_split(pipex.args);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex		pipex;
	int			status;

	if (argc != 5)
		print_error("Cannot create pipe.", EXIT_FAILURE);
	if (pipe(pipex.fd) < 0)
		print_error("fork() failed!", EXIT_FAILURE);
	pipex.pid = fork();
	if (pipex.pid < 0)
		print_error("fork() failed!", EXIT_FAILURE);
	if (pipex.pid == 0)
		child_process(pipex.fd, envp, argv, pipex);
	pipex.pid1 = fork();
	if (pipex.pid1 < 0)
		print_error("fork() failed!", EXIT_FAILURE);
	if (pipex.pid1 == 0)
		child_one_process(pipex.fd, envp, argv, pipex);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	waitpid(pipex.pid, &status, 0);
	waitpid(pipex.pid1, &status, 0);
	return (WEXITSTATUS(status));
}
