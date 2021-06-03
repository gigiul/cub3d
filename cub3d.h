/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalle-a <ldalle-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 14:36:27 by ldalle-a          #+#    #+#             */
/*   Updated: 2021/03/30 15:30:08 by ldalle-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define P	3.14159265359
# define Z  64
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <mlx.h>
# include <math.h>



typedef struct	s_coord {
	int x;
	int y;
}				t_coord;

typedef struct	s_sprite {
	t_coord coord;
	float dist;
	float ra;
	float h;
}				t_sprite;

typedef struct	s_splist {
	t_sprite sprite;
	struct s_splist * next;
} 				t_splist;


typedef struct  s_player {
	float		px;
	float		py;
	float		pdx;
	float		pdy;
	float		pa;
}				t_player;

typedef struct	s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_data;


typedef struct	s_img {
	t_data data;
	char *path;
	int w;
	int h;
}				t_img;

typedef struct	s_texture {
	t_img NO;
	t_img SO;
	t_img WE;
	t_img EA;
	t_img S;
}				t_texture;

typedef struct	s_color {
	int R;
	int G;
	int B;
}				t_color;

typedef struct	s_map {
	int x;
	int y;
	char **map;
}				t_map;

typedef struct	s_infocub {
	char 		*cub;
	t_coord		res;
	t_texture 	texture;
	t_color 	F;
	t_color 	C;
	t_map 		map;
	t_splist 	splist;
}				t_infocub;

typedef struct  s_vars {
	void		*mlx;
	void		*win;
	t_player 	player;
	t_infocub   infocub;
	float		*buff;
	char		key[8];
}				t_vars;

typedef struct  s_dist {
	float		l;
	float		d;
	int			c;
}				t_dist;
typedef struct  s_triangle {
	float	a;
	float	b;
	float	c;
	float	d;
	float	B;
}				t_triangle;

int ft_parsemap(t_infocub *infocub);

int			get_next_line(int fd, char **line);

void		*ft_memmove(void *dst, const void *src, size_t len);
size_t		ft_strlen(const char *s);
char		*ft_join(char const *s1, char const *s2);
int			new_line(char *str);
char		*ft_cpy_line(char *s);
char		*ft_clear(char *s);
void	screenshot(t_vars *vars, char *img);
void	*ft_calloc(size_t count, size_t size);

#endif