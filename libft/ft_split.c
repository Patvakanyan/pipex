/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alen <alen@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:52:43 by apatvaka          #+#    #+#             */
/*   Updated: 2025/03/22 18:15:02 by alen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		in_word = 0;
		while (*s == c && *s)
			++s;
		while (*s != c && *s)
		{
			helper1(&s);
			if (!in_word)
			{
				in_word = 1;
				++count;
			}
			++s;
		}
	}
	return (count);
}

static int	malloc_words(char **ret_str, int index, size_t len)
{
	int	i;

	i = 0;
	ret_str[index] = (char *)malloc(len);
	if (!ret_str[index])
	{
		while (i < index)
		{
			free(ret_str[i]);
			++i;
		}
		free(ret_str);
		return (1);
	}
	return (0);
}

static size_t	get_word_length(char **s, char separator)
{
	size_t	len;

	len = 0;
	if (**s == '\'')
	{
		(*s)++;
		while (**s != '\'' && **s)
		{
			len++;
			(*s)++;
		}
	}
	else
	{
		while (**s != separator && **s != '\'' && **s)
		{
			len++;
			(*s)++;
		}
	}
	return (len);
}

static int	process_words(char **ret_str, char *s, char separator)
{
	int		index;
	size_t	len;

	index = 0;
	while (*s)
	{
		while (*s == separator && *s)
			s++;
		if (!*s)
			break ;
		len = get_word_length(&s, separator);
		if (len > 0)
		{
			if (malloc_words(ret_str, index, len + 1))
				return (1);
			ft_strlcpy(ret_str[index], s - len, len + 1);
			index++;
		}
	}
	return (0);
}

char	**ft_split(char *s, char c)
{
	char	**ret_str;
	int		count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	ret_str = (char **)malloc((count + 1) * sizeof(char *));
	if (!ret_str)
		return (NULL);
	ret_str[count] = NULL;
	if (process_words(ret_str, s, c))
		return (NULL);
	return (ret_str);
}
