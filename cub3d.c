/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldalle-a <ldalle-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 13:16:34 by ldalle-a          #+#    #+#             */
/*   Updated: 2021/04/30 13:16:36 by ldalle-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int checkval(int x, int y,t_map map)
{
	return ((0 <= x && x < map.x) && (0 <= y && y < map.y));
}

int		ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int            get_color(t_data *data, int x, int y)
{
	char    *dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int*)dst);
}

void			my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int rgbToHex (t_color c) {
	int r;
	r = c.B;
	r += c.G * 256;
	r += c.R * 65536;
	return (r);
}

void drawpix(t_data *img, float x, float y,int color, int size)
{
	int i = 0;
	int j;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			my_mlx_pixel_put(img,(int) (x*Z+i+((Z-size)/2)), (int)(y*Z+j+((Z-size)/2)), color);
			j++;
		}
		i++;
	}
}

t_coord getcoord(t_player player)
{
	t_coord coord;
	coord.x = (int)(player.px);
	coord.y = (int)(player.py);
	return(coord);
}
float radToDeg(float a) {return a*180/P;}
float degToRad(int a) { return a*P/180.0;}
float degToRad2(float a) { return a*P/180.0;}
int FixAng(int a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}
float FixAng2(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}
char toggle(char i){if (i == '0') { return('1');}else {return('0');}}
float mod(float i) {return((i > 0) ? i : -i);}
float fix_fisheye(t_vars *vars, float r, float ray) {int ca=FixAng2(vars->player.pa-r); return (ray*cos(degToRad(ca)));}
float fix_notfisheye(t_vars *vars, float r, float ray) {int ca=FixAng2(vars->player.pa-r); return (ray/cos(degToRad(ca)));}
float twopoint(t_coord c, t_player player)
{
	return(sqrt(pow(c.x+0.5 - player.py, 2) + pow(c.y+0.5 - player.px, 2)));
}

void	init_sprite(t_splist *lst, t_vars *vars)
{
	float dx;
	float dy;
	while (lst)
	{
		dx = lst->sprite.coord.y + 0.5 - vars->player.px;
		dy = lst->sprite.coord.x + 0.5 - vars->player.py;
		lst->sprite.dist = twopoint(lst->sprite.coord, vars->player);
		lst->sprite.ra = (-1.0 * atan2(dy, dx));
		lst->sprite.h = (vars->infocub.res.y*2)/lst->sprite.dist;
		lst = lst->next;
	}
}

void sort_list(t_vars *vars)
{
	t_sprite a;
	t_splist *temp1;
	t_splist *temp2;
	if (vars->infocub.splist.sprite.coord.x != 0) {
		for (temp1 = &vars->infocub.splist;
			 temp1 != NULL; temp1 = temp1->next) {
			for (temp2 = temp1->next; temp2 != NULL; temp2 = temp2->next) {
				if (temp2->sprite.dist > temp1->sprite.dist) {
					a = temp1->sprite;
					temp1->sprite = temp2->sprite;
					temp2->sprite = a;
				}
			}
		}
	}
}
void draw_lines_sprite(t_vars *vars, t_splist *lst, float strip, t_data *img, int o)
{
	int h = vars->infocub.res.y;
	float length = lst->sprite.h;
	float ty_step = length;
	float ty_off = 0;
	ty_step = vars->infocub.texture.S.h/ty_step;
	if (length > h) {ty_off = (length - h)/2 ;length = h;};
	int offset = (h - length)/2;
	int i = 0;
	int c;
	float ty = ty_off * ty_step;
	while (i < h) {
		if (i > offset && i < length + offset) {
			c = get_color(&vars->infocub.texture.S.data, (int)((o/lst->sprite.h)*vars->infocub.texture.S.w),(int)ty);
			if (((int) strip - (int) (length / 2) + o > 0 && (int) strip - (int) (length / 2) + o < vars->infocub.res.x) && vars->buff[(int) strip - (int) (length / 2) + o] > lst->sprite.dist) {
				(c == 0x0000000)?: my_mlx_pixel_put(img, (int) strip - (int) (length / 2) + o, i, c);
			}
			ty += ty_step;
		}
		i++;
	}
}

void draw_sprite(t_vars *vars, t_data img)
{
	float strip;
	int i;
	t_splist *lst;
	lst = &vars->infocub.splist;
	while (lst) {
		strip = (radToDeg(lst->sprite.ra) + vars->player.pa);
		strip = FixAng2(strip);
		strip += 30;
		strip = FixAng2(strip);
		 if (strip >= 330.0)
		 strip = strip - 360.0;
		strip = vars->infocub.res.x -  (strip *( vars->infocub.res.x) / 60);
		i = 0;
		while (i < lst->sprite.h)
			draw_lines_sprite(vars, lst, strip, &img, i++);
		lst = lst->next;
	}
}

void sprite (t_vars *vars)
{
	init_sprite(&vars->infocub.splist, vars);
	sort_list(vars);
}

void	ft_initposition(float x, float y, char c,t_player *player)
{
	player->px = x+0.5;
	player->py = y+0.5;
    if (c == 'N')
        player->pa = 270;
    if (c == 'S')
        player->pa = 90;
    if (c == 'E')
        player->pa = 0;
    if (c == 'W')
        player->pa = 180;
	player->pdx = cos(degToRad(player->pa))*1;
	player->pdy = -sin(degToRad(player->pa))*1;
}

t_dist drawRays(t_vars *vars, float a)
{
	t_coord coord;
	t_triangle triH, triV;
	t_player nextcoord;
	t_player storecoord;
	int i = 0;
	coord = getcoord(vars->player);
	/* Vertical */
	triH.b = vars->player.py - coord.y;
	triH.B = a;
	if (180 <= triH.B && triH.B < 359)
		triH.B -= 180;
	else if (0 <= triH.B && triH.B < 180)
		triH.b = 1 - triH.b;
	triH.a = triH.b/sin(degToRad2(triH.B));
	triH.d = 1/sin(degToRad2(triH.B));
	triH.c = sqrt((pow(triH.a, 2) - pow(triH.b, 2)));
	triH.c *= (a > 270 || a <= 90) ? 1 : -1;
	nextcoord = vars->player;
	nextcoord.px += triH.c;
	nextcoord.py += (a > 180)? -1 : 1;
	triH.c = sqrt((pow(triH.d, 2) - pow(1, 2)));
	triH.c *= (a > 270 || a <= 90) ? 1 : -1;
	while (i < vars->infocub.map.x)
	{
		if (checkval((int)nextcoord.py, (int)nextcoord.px, vars->infocub.map) && vars->infocub.map.map[(int)nextcoord.py][(int)nextcoord.px] == '1')
			i = vars->infocub.map.x;
		else {
			nextcoord.px += triH.c;
			nextcoord.py += (a > 180) ? -1 : 1;
			triH.a += triH.d;
		}
		i++;
	}
	storecoord.px = nextcoord.px;
	/* Horizontal */
	triV.b = vars->player.px - coord.x;
	triV.B = a;
	
	if (270 <= triV.B || triV.B < 90)
		triV.b = 1 - triV.b;
	else if (90 < triV.B && triV.B < 270)
		triV.B -= 180;
	triV.a = triV.b/cos(degToRad2(triV.B));
	triV.d = 1/cos(degToRad2(triV.B));
	triV.c = sqrt((pow(triV.a, 2) - pow(triV.b, 2)));
	triV.c *= (a > 180) ? -1 : 1;
	nextcoord = vars->player;
	nextcoord.py += triV.c;
	nextcoord.px += (a > 270 || a <= 90) ? 1 : -1;
	triV.c = sqrt((pow(triV.d, 2) - pow(1, 2)));
	triV.c *= (a > 180) ? -1 : 1;
	i = 0;
	while (i < vars->infocub.map.y)
	{
		if (checkval((int)nextcoord.py, (int)nextcoord.px, vars->infocub.map) && vars->infocub.map.map[(int)nextcoord.py][(int)nextcoord.px] == '1')
			i = vars->infocub.map.y;
		else {
			nextcoord.py += triV.c;
			nextcoord.px += (a > 270 || a <= 90) ? 1 : -1;
			triV.a += triV.d;
		}
		i++;
	}
	storecoord.py = nextcoord.py;
	t_dist dist;
	/*
	0 N
	1 S
	2 E
	3 W
	4 Sprite
	*/
	if (triV.a < 0) {triV.a = vars->infocub.map.x;}
	if (triH.a < 0) {triH.a = vars->infocub.map.y;}
	if (triV.a < triH.a)
	{
		if (90 < a && a < 270)
			dist.c = 2;//west
		else
			dist.c = 3;//east
		dist.d = storecoord.py - (int)storecoord.py;
		dist.l = fix_fisheye(vars, a, triV.a);
	}
	else
	{
		if (a < 180)
			dist.c = 0;//north
		else
			dist.c = 1;//south
		dist.d = storecoord.px - (int)storecoord.px;
		dist.l = fix_fisheye(vars, a, triH.a);
	}
	return (dist);
}

void    drawplayer(t_vars *vars, t_data *img)
{
    drawpix(img, vars->player.px-0.5, vars->player.py-0.5, 0x00fc0303, 2);
    t_dist m;

    for (float r = vars->player.pa - 30; r <= vars->player.pa + 30; r = r + 1)
    {
    	m = drawRays(vars, FixAng2(r));
		m.l = fix_notfisheye(vars, r, m.l);
		for (float i = m.l; i > 0; i-= 0.01)
    		drawpix(img, (vars->player.px-0.5)+cos(degToRad2(r))*i, (vars->player.py-0.5)+(-sin(degToRad2(r))-(2*(-sin(degToRad2(r)))))*i, 0x00fc0303, 3);
    }
}

void 	*drawmap2d(t_vars *vars)
{
	t_data  img;
    static int q = 0;
	img.img  = mlx_new_image(vars->mlx, vars->infocub.res.x, vars->infocub.res.y);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	int i = 0;
	int j;
	while (i < vars->infocub.map.x)
	{
		j = 0;
		while (j < vars->infocub.map.y)
		{
			if (vars->infocub.map.map[i][j] == '1')
				drawpix(&img, j , i, 0x00fff700, 63);
			else if (vars->infocub.map.map[i][j] == '2')
				drawpix(&img, j , i, 0x0027f011, 20);
			else if (vars->infocub.map.map[i][j] == 'N' || vars->infocub.map.map[i][j] == 'S' || vars->infocub.map.map[i][j] == 'E' || vars->infocub.map.map[i][j] == 'W')
				{
                    (q) ? : ft_initposition((float)j, (float)i , vars->infocub.map.map[i][j], &vars->player);
					drawplayer(vars, &img);
					q = 1;
				}
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
	return (img.img);
}

void drawlines(t_data *img, t_vars *vars, float length, int p, float ry, int card) {
	int h = vars->infocub.res.y;
	float ty_step = length;
	if(card == 0)
		ty_step = vars->infocub.texture.NO.h/ty_step;
	else if(card == 1)
		ty_step = vars->infocub.texture.SO.h/ty_step;
	else if(card == 2)
		ty_step = vars->infocub.texture.EA.h/ty_step;
	else if(card == 3)
		ty_step = vars->infocub.texture.WE.h/ty_step;
	float ty_off = 0;
	if (length > h) {ty_off = (length - h)/2 ;length = h;};
	int offset = (h - length)/2;
	int i = 0;
	int c;
	float ty = ty_off * ty_step;
	float tx = ry;
	while (i < h)
	{
		if (i < offset)
			my_mlx_pixel_put(img,  p, i,rgbToHex(vars->infocub.C));
		else if (i < length+offset)
		{
			if(card == 0)
				c = get_color(&vars->infocub.texture.NO.data, (int)(tx*vars->infocub.texture.NO.w),(int)ty);
			else if(card == 1)
				c = get_color(&vars->infocub.texture.SO.data, (int)(tx*vars->infocub.texture.SO.w),(int)ty);
			else if(card == 2)
				c = get_color(&vars->infocub.texture.EA.data, (int)(tx*vars->infocub.texture.EA.w),(int)ty);
			else if(card == 3)
				c = get_color(&vars->infocub.texture.WE.data, (int)(tx*vars->infocub.texture.WE.w),(int)ty);
			my_mlx_pixel_put(img, p, i, c);
			ty+= ty_step;
		}
		else
			my_mlx_pixel_put(img, p, i,rgbToHex(vars->infocub.F));
		i++;
	}
}

char 	*drawmap3d(t_vars *vars)
{
	t_data  img;
	t_dist m;
	int i = 0;
	img.img  = mlx_new_image(vars->mlx, vars->infocub.res.x, vars->infocub.res.y);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	vars->buff = (float*) ft_calloc(vars->infocub.res.x, sizeof(float));
	for (float r = vars->player.pa-30; r < vars->player.pa+30; r = r + (60.0/vars->infocub.res.x))
	{
		m = drawRays(vars, FixAng2(r));
		drawlines(&img, vars, (vars->infocub.res.y*2)/m.l, i, m.d, m.c);
		vars->buff[i] = m.l;
		i++;
	}
	sprite(vars);
	draw_sprite(vars, img);
	mlx_put_image_to_window(vars->mlx, vars->win, img.img, 0, 0);
	return (img.addr);
}

int		close_wnd(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
}

void error()
{
	write(1, "Error\n", 6);
}

int				keypressed(int key, t_vars *vars)
{
	if  (key==123)	{vars->key[5] = '1';}
    if  (key==124)	{vars->key[6] = '1';}

	if  (key==2)	{vars->key[2] = '1';}
	if  (key==0)	{vars->key[0] = '1';}

	if  (key==13)	{vars->key[3] = '1';}
    if  (key==1)	{vars->key[1] = '1';}
    if 	(key==14)	{vars->key[4] = toggle(vars->key[4]);}
	if (key == 53)
	{
		close_wnd(vars);
	}
	return (0);
}
int				keyrelease(int key, t_vars *vars)
{

	if (key==123)	{vars->key[5] = '0';}
    if (key==124)	{vars->key[6] = '0';}
	if (key==2)		{vars->key[2] = '0';}
	if (key==0)		{vars->key[0] = '0';}
	if (key==13)	{vars->key[3] = '0';}
    if (key==1)		{vars->key[1] = '0';}

	return (0);
}
int				mainloop(t_vars *vars)
{
	if (vars->key[4] == '1')
		drawmap2d(vars);
	else
		drawmap3d(vars);
	if (vars->key[5] == '1')	{vars->player.pa-=2; vars->player.pa=FixAng(vars->player.pa); vars->player.pdx=cos(degToRad(vars->player.pa)); vars->player.pdy=-sin(degToRad(vars->player.pa));}
	if (vars->key[6] == '1')	{vars->player.pa+=2; vars->player.pa=FixAng(vars->player.pa); vars->player.pdx=cos(degToRad(vars->player.pa)); vars->player.pdy=-sin(degToRad(vars->player.pa));}

	if (vars->key[2] == '1')	{vars->player.px+=vars->player.pdy*0.1; vars->player.py+=vars->player.pdx*0.1;}
	if (vars->key[0] == '1')	{vars->player.px-=vars->player.pdy*0.1; vars->player.py-=vars->player.pdx*0.1;}

	if (vars->key[3] == '1')	{vars->player.px+=vars->player.pdx*0.1; vars->player.py-=vars->player.pdy*0.1;}
	if (vars->key[1] == '1')	{vars->player.px-=vars->player.pdx*0.1; vars->player.py+=vars->player.pdy*0.1;}

	return(1);
}

void get_texture(t_texture *texture ,void *mlx)
{
	texture->NO.data.img = mlx_xpm_file_to_image(mlx, texture->NO.path, &texture->NO.w, &texture->NO.h);
	texture->NO.data.addr = mlx_get_data_addr(texture->NO.data.img, &texture->NO.data.bits_per_pixel, &texture->NO.data.line_length, &texture->NO.data.endian);
	
	texture->SO.data.img = mlx_xpm_file_to_image(mlx, texture->SO.path, &texture->SO.w, &texture->SO.h);
	texture->SO.data.addr = mlx_get_data_addr(texture->SO.data.img, &texture->SO.data.bits_per_pixel, &texture->SO.data.line_length, &texture->SO.data.endian);
	
	texture->WE.data.img = mlx_xpm_file_to_image(mlx, texture->WE.path, &texture->WE.w, &texture->WE.h);
	texture->WE.data.addr = mlx_get_data_addr(texture->WE.data.img, &texture->WE.data.bits_per_pixel, &texture->WE.data.line_length, &texture->WE.data.endian);
	
	texture->EA.data.img = mlx_xpm_file_to_image(mlx, texture->EA.path, &texture->EA.w, &texture->EA.h);
	texture->EA.data.addr = mlx_get_data_addr(texture->EA.data.img, &texture->EA.data.bits_per_pixel, &texture->EA.data.line_length, &texture->EA.data.endian);
	
	texture->S.data.img = mlx_xpm_file_to_image(mlx, texture->S.path, &texture->S.w, &texture->S.h);
	texture->S.data.addr = mlx_get_data_addr(texture->S.data.img, &texture->S.data.bits_per_pixel, &texture->S.data.line_length, &texture->S.data.endian);
}

int				main(int argc, char **argv)
{
	t_vars	  	vars;
	vars.key[7] = '0';
	vars.key[1] = '0';
	vars.key[2] = '0';
	vars.key[3] = '0';
	vars.key[4] = '0';
	vars.key[5] = '0';
	vars.key[6] = '0';
	vars.key[7] = '\0';
	if (1 < argc && 4 > argc)
	{
		vars.infocub.cub = argv[1];
		if(!ft_parsemap(&vars.infocub)) {
			error();
			return (0);
		}
		vars.mlx = mlx_init();
		vars.win = mlx_new_window(vars.mlx, vars.infocub.res.x, vars.infocub.res.y, "Hello world!");
		get_texture(&vars.infocub.texture, vars.mlx);
		mlx_hook(vars.win, 2, 1L<<0, keypressed, &vars);
		mlx_hook(vars.win, 3, 1L<<1, keyrelease, &vars);
		mlx_hook(vars.win, 17, 0L, close_wnd, &vars);
		mlx_loop_hook(vars.mlx, mainloop, &vars);
		char *p;
		drawmap2d(&vars);
		p = drawmap3d(&vars);
		if (argc == 3 && !ft_strcmp(argv[2], "--save"))
			screenshot(&vars, p);
		mlx_loop(vars.mlx);
	} else {
		error();
		return (0);
	}
	return (0);
}