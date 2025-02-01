/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 15:08:06 by meabdelk          #+#    #+#             */
/*   Updated: 2025/02/01 18:26:38 by akoraich         ###   ########.fr       */
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

#define screenWidth 955
#define screenHeight 655

typedef struct s_map t_map;
typedef struct s_data t_data;

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
    char **map_copy;
     int x_p;
    int y_p;
    char *file_name;
    char *no_texture;
    char *so_texture;
    char *we_texture;
    char *ea_texture;
    int f_color[3];
    int c_color[3];
    int countlines_map;
    int s;
    int w;
    int e;
    int n;
    t_data *data;
    t_count *count;
} t_map;

typedef struct s_wall
{
    float line_length;
    float draw_start;
    float draw_end;
}              t_wall;

typedef struct s_mini_map
{
    int height;
    int width;
    int start;
    int square_width;
    int square_height;
}              t_mini_map;

typedef struct s_data
{
    float posx ;
    float posy ;
    float dirx ;
    float diry ;
    float planeX ;
    float planeY ;
    float cameraX;
    float raydirX;
    float raydirY;
    int mapX;
    int mapY;
    float sidedistX;
    float sidedistY;
    float deltadisX;
    float deltadisY;
    float prepwalldist;
    int stepx;
    int stepy;
    int hit;
    int side;
    char compass;
    void *mlx;
    void *mlx_win;
    int player_i;
    int player_j;
    t_map *map;
    t_wall *wall;
    t_mlx *img;
    t_mini_map *minimap;

} t_data;


int	ft_strcmp(char *s1, char *s2);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strchar(char *s, int c);
void free_all(t_map *map);
void player_init(t_data *data);
void raycast(t_data *data);
void ray_init(t_data *data);
void calc_length(t_data *data);
void dda(t_data *data);
void set_dda_vars(t_data *data);
void wall_init(t_data *data);
void ray_data_init(t_data *data, t_map *map, t_wall *wall);
int draw_a_line(t_data *data, int x);
void	my_mlx_pixel_put(t_mlx *data, int x, int y, int color);
void data_init(t_data *data);
void create_image(t_data *data);
int draw_minimap(t_data *data, t_mini_map *map);
void mini_map(t_data *data);
void check_player(t_map *map, int j, int i);


int check_path(const char *path);
void check_map(t_map *map);
void check_valid(char *line, char **path);
void free_text(char **texture);
char	**ft_split(char const *s, char c);
void ini(t_map *map);
int count_part(char **value);
void check_first_last(t_map *map, int *i);
int skp_spaces(char *line);
void check_map_borders(t_map *map, int *i);
void check_characters(t_map *map, int *i);
void ft_error(t_map *map);
void file_err(int i);
void check_spaces(t_map *map);
void free_map2(t_map *map);
char	*ft_strtrim(char const *s1, char const *set);
int	ft_atoi(const char *str);
int check_digit(char *value);
void	*ft_memset(void *str, int c, size_t n);
int count_comma(char *value);
void parse_map(t_map *map, int i);
int rgb_c(t_data *data);

void draw_fc(t_data *data, int x);


#endif
