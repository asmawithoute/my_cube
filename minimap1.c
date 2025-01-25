/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 13:50:58 by akoraich          #+#    #+#             */
/*   Updated: 2025/01/25 16:44:57 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void draw_square(t_data *data, int mapx, int mapy, int x, int y)
{
    int S_x = x;
    int S_y = y;
    int color;

    if (data->map->map[mapx][mapy] == '1')
        color = 0x004287f5;
    else if (data->map->map[mapx][mapy] == 'E' || data->map->map[mapx][mapy] == 'W' || data->map->map[mapx][mapy] == 'N' || data->map->map[mapx][mapy] == 'S')
        color = 0x00361227;
    else if (data->map->map[mapx][mapy] == '0')
        color = 0x007b547d;
    // printf("mapx %d mapy %d char %c X %d Y %d color %d\n", mapx, mapy, data->map->map[mapx][mapy], *x, *y, color);
    while (y < (S_y + data->minimap->square_height))
    {
        x = S_x;
        while (x < (S_x + data->minimap->square_width))
        {
            printf("*x %d, *y %d\n", x, y);
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
    // int i;
    // int j;

    x = map->start;
	y = 20;
    // j = data->map->map_j;
    // i = data->map->map_i;
    mapx = 0;
    mapy = 0;
    printf("rororororor w %d, h %d, x %d, \n", map->width, map->height, map->start);
	while (mapx < data->map->map_i )
	{
        while (mapy < data->map->map_j )
        {
            draw_square(data, mapx, mapy, x ,y);
            x = x + map->square_width;
            mapy++;
            // my_mlx_pixel_put(data->img, x, y, 0x007b547d);
            // x++;
        }
        // y++;
        y = y + map->square_height;
        x = map->start;
        mapy = 0;
        mapx++;
        // x = map->start;
        // if(data->side == 0)
		//     my_mlx_pixel_put(data->img, x, y, 0x007b547d);
        // else
		//     my_mlx_pixel_put(data->img, x, y, 0x00912020);
		// y++;
	}
    return 0;
}

void mini_map(t_data *data)
{
    t_mini_map *minimap;
    
    minimap = malloc(sizeof(t_mini_map));

    // minimap->width = data->map->map_j * 3 ;
    minimap->width = screenWidth / 9 ;
    printf("mini width %d \n", minimap->width);
    minimap->height = screenHeight / 9;
    printf("mini height %d \n", minimap->height);
    minimap->start = 20;
    minimap->square_width = minimap->width / data->map->map_j;
    printf("mini sq width %d map_j %d \n", minimap->square_width, data->map->map_j);
    minimap->square_height = minimap->height / data->map->map_i;
    printf("mini sq height %d map_i %d \n", minimap->square_height, data->map->map_i);

    // minimap->square_width = 30;
    // minimap->square_height = 30;
    
    // minimap->end = 0;
    data->minimap = minimap;
    // draw_minimap(data, minimap);
}
