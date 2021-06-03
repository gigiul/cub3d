/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsemap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalle-a <ldalle-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 13:17:12 by ldalle-a          #+#    #+#             */
/*   Updated: 2021/04/30 13:17:14 by ldalle-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		ft_atoi(char **str) {
	int result;

	result = 0;
	while (!(**str >= '0' && **str <= '9'))
		(*str)++;
	while (**str >= '0' && **str <= '9') {
		result *= 10;
		result += **str - 48;
		(*str)++;
	}
	return (result);
}

int ft_find(char *str, char *c)
{
	int b;

	b = 0;
	while (str[b] == c[b])
		b++;
	return (b == (int)ft_strlen(c));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*newstring;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2 || !(newstring = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 2)))
		return (0);
	while (s1[i] != '\0')
	{
		newstring[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		newstring[i] = s2[j];
		i++;
		j++;
	}
	newstring[i++] = '\n';
	newstring[i] = '\0';
	return (newstring);
}

int		ft_maxline(char *s)
{
	int i;
	int j;
	int max;

	i = 0;
	max = 0;
	while (s[i])
	{
		j = 0;
		while(s[i] != '\n')
			{
				j++;
				i++;
			}
		if (j > max)
			max = j;
		i++;
	}	
	return (max);
}

void	*ft_memset(void *b, int c, size_t len)
{
	char	*str;
	size_t	cont;

	str = (char *)b;
	cont = 0;
	while (cont < len)
	{
		str[cont] = (unsigned char)c;
		cont++;
	}
	return (b);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	totsize;
	void	*dst;

	totsize = size * count;
	if (!(dst = malloc(totsize)))
		return (0);
	else
		ft_memset(dst, ' ', totsize);
	return (dst);
}

void ft_printmatrix(t_map map)
{
	for (int j = 0; j < map.x; j++)
		printf("%s\n", map.map[j]);
	printf("%d::::::::::::::::::%d", map.x, map.y );
}


int IsOk(char *str)
{
	while (*str)
		if (*str++ == '0')
			return(0);
	return(1);
}

int checkcard(t_coord p, t_map map)
{
	char card[4] = "0000";
	int i;

	i = 0;
	while (p.x - i >= 0)
	{
		if (map.map[p.x - i][p.y] == '1')
			card[0] = '1';
		i++;
	}

	i = 0;
	while (p.x + i < map.x)
	{
		if (map.map[p.x + i][p.y] == '1')
			card[1] = '1';
		i++;
	}
	i = 0;
	while (p.y + i < map.y)
	{
		if (map.map[p.x][p.y + i] == '1')
			card[2] = '1';
		i++;
	}
	i = 0;
	while (p.y - i >= 0)
	{
		if (map.map[p.x][p.y - i] == '1')
			card[3] = '1';
		i++;
	}
	return (IsOk(card));
}

int checkplayer(t_map map)
{
	t_coord p;
	int i = 0;
	int j;
	p.x = -1;
	p.y = -1;
	while (i < map.x)
	{
		j=0;
		while(j < map.y)
		{
			if (map.map[i][j] == 'N' || map.map[i][j] == 'S' || map.map[i][j] == 'E' || map.map[i][j] == 'W')
			{
				p.x = i;
				p.y = j;
			}
			j++;
		}
		i++;
	}
	return(checkcard(p, map));
}
int not012(char c)
{
	if (c != '0' && c != '1' && c != '2' &&
	c != 'N' && c != 'S' && c != 'E' && c != 'W')
		return (1);
	return (0);
}
int not012s(char c)
{
	if (c != '0' && c != '1' && c != '2' &&
	c != 'N' && c != 'S' && c != 'E' && c != 'W' && c != ' ')
		return (1);
	return (0);
}


int		check_around(int i, int j, t_map map)
{
	if (not012(map.map[i - 1][j + 1]))
		return (-1);
	if (not012(map.map[i - 1][j - 1]))
		return (-1);
	if (not012(map.map[i + 1][j - 1]))
		return (-1);
	if (not012(map.map[i + 1][j + 1]))
		return (-1);
	if (not012(map.map[i][j - 1]))
		return (-1);
	if (not012(map.map[i][j + 1]))
		return (-1);
	if (not012(map.map[i - 1][j]))
		return (-1);
	if (not012(map.map[i + 1][j]))
		return (-1);
	return (1);
}

int checkclose(t_map map)
{
	t_coord p;
	int i = 0;
	int j;
	p.x = -1;
	p.y = -1;
	while (i < map.x)
	{
		j=0;
		while(j < map.y)
		{
			if ((map.map[i][j] == '0' && (check_around(i, j, map) < 0)))
				return (0);
			if ((map.map[i][j] == '2' && (check_around(i, j, map) < 0)))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int checkchar(t_map map)
{
	int i = 0;
	int j;
	while (i < map.x)
	{
		j=0;
		while(j < map.y)
		{
			if (not012s(map.map[i][j]))
				return(0);
			j++;
		}
		i++;
	}
	return (1);
}

char checkmap(t_map map)
{
	if (checkclose(map) && checkplayer(map) && checkchar(map))
		return('1');
	else
		return('0');
}

t_splist	*ft_lstnew(t_coord *content)
{
	t_splist *res;

	if (!(res = (t_splist *)malloc(sizeof(t_splist))))
		return (0);
	res->sprite.coord = *content;
	res->next = 0;
	return (res);
}

int	ft_lstsize(t_splist *lst)
{
	int count;

	count = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

void	ft_lstadd_front(t_splist **lst, t_splist *new)
{
	t_splist *temp;

	temp = *lst;
	*lst = new;
	new->next = temp;
}

void get_sprite(t_infocub *infocub)
{
	int c = 0;
	int i = 0;
	int j;
	t_coord coord;
	t_splist *splist;
	while (i < infocub->map.x)
	{
		j=0;
		while(j < infocub->map.y)
		{
			if (infocub->map.map[i][j] == '2')
			{
				coord.x = i;
				coord.y = j;
				if (c++ == 0)
					splist = ft_lstnew(&coord);
				else
					ft_lstadd_front(&splist, ft_lstnew(&coord));
			}
			j++;
		}
		i++;
	}
	if (c == 0)
		infocub->splist.sprite.coord.x = 0;
	else
		infocub->splist = *splist;
}

int ft_parsemap(t_infocub *infocub)
{
	int i;
	int j;
	char *line;
	int fd;
	char a[10] = "000000000\0";

	fd = open (infocub->cub , O_RDONLY);
	i = 0;
	while (get_next_line(fd, &line) > 0 && i < 8)
	{
		if (ft_find(line, "R"))
		{
			infocub->res.x = ft_atoi(&line);
			if (infocub->res.x > 2560)
				infocub->res.x = 2880;
			infocub->res.y = ft_atoi(&line);
			if (infocub->res.y > 1440)
				infocub->res.y = 1620;
			a[0] = '1';
		}
		else if (ft_find(line, "F"))
		{
			infocub->F.R = ft_atoi(&line);
			infocub->F.G = ft_atoi(&line);
			infocub->F.B = ft_atoi(&line);
			a[1] = '1';
		}
		else if (ft_find(line, "C"))
		{
			infocub->C.R = ft_atoi(&line);
			infocub->C.G = ft_atoi(&line);
			infocub->C.B = ft_atoi(&line);
			a[2] = '1';
		}
		else if (ft_find(line, "NO"))
		{
			line += 3;
			infocub->texture.NO.path = line;
			a[3] = '1';
		}
		else if (ft_find(line, "SO"))
		{
			line += 3;
			infocub->texture.SO.path = line;
			a[4] = '1';
		}
		else if (ft_find(line, "WE"))
		{
			line += 3;
			infocub->texture.WE.path = line;
			a[5] = '1';
		}
		else if (ft_find(line, "EA"))
		{
			line += 3;
			infocub->texture.EA.path = line;
			a[6] = '1';
		}
		else if (ft_find(line, "S"))
		{
			line += 2;
			infocub->texture.S.path = line;
			a[7] = '1';
		}
		i++;
	}

	i = 0;
	char *save;
	
	save = malloc(1);
	save[0] = 0;
	while (get_next_line(fd, &line) > 0)
	{
		save = ft_strjoin(save, line);
		i++; //numero di colonne
	}
	save = ft_strjoin(save, line);
	infocub->map.x = i + 1;
	infocub->map.y = ft_maxline(save);
	infocub->map.map = (char**) ft_calloc(infocub->map.x, sizeof(char *));
	for (j = 0; j < infocub->map.x; j++)
	{
		i = 0;
		infocub->map.map[j] = (char*) ft_calloc(infocub->map.y, sizeof(char));
		while (infocub->map.map[j][i] && *save != '\n')
		{
			infocub->map.map[j][i] = *save;
			save++;
			i++;
		}
		save++;
	}
	a[8] = checkmap(infocub->map);
	get_sprite(infocub);
	return(IsOk(a));
}

