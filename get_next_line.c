/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalle-a <ldalle-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 13:16:48 by ldalle-a          #+#    #+#             */
/*   Updated: 2021/04/30 13:16:50 by ldalle-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*ft_clear(char *s)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != '\n')
		i++;
	if (!s[i])
	{
		free(s);
		return (0);
	}
	if (!(tmp = malloc(sizeof(char) * ((ft_strlen(s) - i) + 1))))
		return (0);
	j = 0;
	i = i + 1;
	while (s[i])
		tmp[j++] = s[i++];
	tmp[j] = 0;
	free(s);
	return (tmp);
}

int		get_next_line(int fd, char **line)
{
	static char		*save;
	char			*buff;
	int				count;
	int				BUFFER_SIZE = 5;

	count = 1;
	if (fd < 0 || !line)
		return (-1);
	if (!(buff = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	while (!new_line(save) && count != 0)
	{
		if ((count = read(fd, buff, BUFFER_SIZE)) == -1)
		{
			free(buff);
			return (-1);
		}
		buff[count] = '\0';
		save = ft_join(save, buff);
	}
	free(buff);
	*line = ft_cpy_line(save);
	save = ft_clear(save);
	if (count == 0)
		return (0);
	return (1);
}
