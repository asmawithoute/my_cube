/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:50:58 by akoraich          #+#    #+#             */
/*   Updated: 2025/02/09 18:46:10 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw_square(t_data *data, int mapx, int mapy, int x, int y)
{
    int S_x = x;
    int S_y = y;
    int color;
    // printf("mapx %d mapy %d \n", mapx, mapy);
    //      printf("posx %f, pos y %f\nmpox %d, mpo y %d \n ", data->posx, data->posy, data->minimap->mini_posx, data->minimap->mini_posy);
    if (data->map->map[mapx][mapy] == '1')
        color = 0x00214780;
    else if (mapy == data->minimap->mini_posx && mapx == data->minimap->mini_posy)
        color = 0x00fc0317;
    else if (data->map->map[mapx][mapy] == '0')
        color = 0x007b547d;
    //  %c X %d Y %d color %d\n", mapx, mapy, data->map->map[mapx][mapy], *x, *y, color);
    while (y < (S_y + data->minimap->square_height))
    {
        x = S_x;
        while (x < (S_x + data->minimap->square_width))
        {
            // printf("*x %d, *y %d\n", x, y);
            my_mlx_pixel_put(data->img, x, y, color);
            x++;
        }
        y++;
    }
}
// my_mlx_pixel_put(data->img, x, y, 0x00361227);

int draw_minimap(t_data *data, t_mini_map *map)
{
    int x;
	int y;
    int mapx;
    int mapy;

    x = map->x_start;
	y = map->y_start;
    mapx = 0;
    mapy = 0;
    //printf("rororororor w %d, h %d, x %d, \n", map->width, map->height, map->start);
	while (mapx < data->map->map_i)
	{
        while (mapy < data->map->map_j)
        {
            // printf("mapx %d mapy %d mapj %d\n", mapx, mapy, data->map->map_j);
            draw_square(data, mapx, mapy, x ,y);
            x = x + map->square_width;
            mapy++;
        }
        y = y + map->square_height;
        x = map->x_start;
        mapy = 0;
        mapx++;
	}
    return 0;
}

// void get_mini_coord(t_data *data, t_mini_map *map)
// {
    
// }

void mini_map(t_data *data)
{
    t_mini_map *minimap;

    minimap = malloc(sizeof(t_mini_map));
    // minimap->width = data->map->map_j * 3 ;
    minimap->width = screenWidth / 9;
    // printf("screen width %d, mini width %d \n",screenWidth, minimap->width);
    minimap->height = screenHeight / 9;
    // printf("screen height %d, mini height %d \n",screenHeight, minimap->height);
    minimap->x_start = screenWidth / 30;
    minimap->y_start = screenHeight / 20;
    minimap->square_width = minimap->width / data->map->map_j;
    // printf("mini sq width %d map_j %d \n", minimap->square_width, data->map->map_j);
    minimap->square_height = minimap->height / data->map->map_i;
    if (minimap->square_width < 2)
        minimap->square_width = 2;
    if (minimap->square_height < 2)
        minimap->square_height = 2;
    minimap->mini_posx = (int)data->posx;
    minimap->mini_posy = (int)data->posy;
    // printf("mini sq height %d map_i %d \n", minimap->square_height, data->map->map_i);
    // minimap->square_width = 30;
    // minimap->square_height = 30;

    // minimap->end = 0;
    // minimap->flag = 0;
    // minimap->area = minimap->height * minimap->width;

    data->minimap = minimap;
    // get_mini_coord(data, minimap);
    // draw_minimap(data, minimap);
}
