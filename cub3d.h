/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 15:08:06 by meabdelk          #+#    #+#             */
/*   Updated: 2024/12/16 15:45:15 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "get_next_line/get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <mlx.h>
#include <math.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1400
#define screenHeight 1800

typedef struct	s_mlx
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;

}				t_mlx;


typedef struct s_count
{
    int no_count;
    int so_count;
    int we_count;
    int ea_count;
    int f_count;
    int c_count;
    
}  t_count;

typedef struct s_map
{
    char **line;
    int countlines;
    int map_i;
    int map_j;
    char **map;
    
    t_count *count;
    
} t_map;

typedef struct s_wall
{
    int line_length;
    int draw_start;
    int draw_end;
}              t_wall;

typedef struct s_data
{
    int posx ;
    int posy ;
    int dirx ;
    int diry ;
    double planeX ;
    double planeY ;
    double cameraX;
    double raydirX;
    double raydirY;
    int mapX;
    int mapY;
    double sidedistX;
    double sidedistY;
    double deltadisX;
    double deltadisY;
    double prepwalldist;
    int stepx;
    int stepy;
    int hit;
    int side;
    char compass;
    t_map *map;
    t_wall *wall;
    t_mlx *img;
    
} t_data;


int	ft_strcmp(char *s1, char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strchar(char *s, int c);
void free_all(t_map *map);

#endif
