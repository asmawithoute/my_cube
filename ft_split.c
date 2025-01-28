/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:33:41 by akoraich          #+#    #+#             */
/*   Updated: 2025/01/27 11:34:11 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*ft_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(end - start + 1);
	if (!word)
		return (0);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

void	ft_free(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static char	**set_word(const char *str, char c, int l)
{
	int		i;
	char	**p;
	int		start;
	int		j;

	i = 0;
	j = 0;
	start = 0;
	p = (char **)malloc(sizeof(char *) * (l + 1));
	if (!p)
		return (NULL);
	while (j < l)
	{
		while (str[i++] == c)
			start = i;
		while (str[i] != c && str[i])
			i++;
		p[j] = ft_word(str, start, i);
		if (!p[j])
			ft_free(p, j);
		j++;
	}
	p[j] = NULL;
	return (p);
}

char	**ft_split(char const *s, char c)
{
	char	**word;
	int		len;

	if (s == NULL)
		return (NULL);
	len = count_words(s, c);
	word = set_word(s, c, len);
	return (word);
}
