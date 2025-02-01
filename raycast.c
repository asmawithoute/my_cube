/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:44:11 by akoraich          #+#    #+#             */
/*   Updated: 2025/02/01 19:09:48 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void data_init(t_data *data)
{
    //printf("heyyyyyyy \n");
    //depends on players pos NWSE
    if (data->compass == 'N')
    {
        data->dirx = 0;
        data->diry = -1;
        data->planeX = 0.66;
        data->planeY = 0;
		//printf("compass is %c and dirx is %f diry is %f and planex %f, planey is %f\n", data->compass, data->dirx, data->diry, data->planeX, data->planeY);
    }
    else if (data->compass == 'S')
    {
        data->dirx = 0;
        data->diry = 1;
        data->planeX = -0.66;
        data->planeY = 0;
		//printf("compass is %c and dirx is %f diry is %f and planex %f, planey is %f\n", data->compass, data->dirx, data->diry, data->planeX, data->planeY);

    }
    else if (data->compass == 'E')
    {
        data->dirx = 1;
        data->diry = 0;
        data->planeX = 0;
        data->planeY = 0.66;
		//printf("compass is %c and dirx is %f diry is %f and planex %f, planey is %f\n", data->compass, data->dirx, data->diry, data->planeX, data->planeY);

    }
    else if (data->compass == 'W')
    {
        data->dirx = -1;
        data->diry = 0;
        data->planeX = 0;
        data->planeY = -0.66;
		//printf("compass is %c and dirx is %f diry is %f and planex %f, planey is %f\n", data->compass, data->dirx, data->diry, data->planeX, data->planeY);

    }
    else
        printf("errooorororororor\n");
}

// int *game_in(t_data *data)
// {
   
// }

void	 my_mlx_pixel_put(t_mlx *img, int x, int y, int color)
{
	char	*dst;

    // dst = malloc(sizeof(img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8))));

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int draw_a_line(t_data *data, int x)
{
	int y;
    //printf("got here x is %d, start is %d end is %d, side is %d and line_length is %d\n", x, data->wall->draw_start, data->wall->draw_end, data->side, data->wall->line_length);

    // if (data->wall->line_length < 100)
    // {
    //     return 1;
        
    // }
	y = data->wall->draw_start;
	while (y < data->wall->draw_end)
	{
        if(data->side == 0)
		    my_mlx_pixel_put(data->img, x, y, 0x007b547d);
        else
		    my_mlx_pixel_put(data->img, x, y, 0x00912020);
		y++;
	}
    return 0;
}

void ray_data_init(t_data *data, t_map *map, t_wall *wall)
{
    data->cameraX = 0;
    data->deltadisX = 0;
    data->deltadisY = 0;
    data->dirx = 0;
    data->diry = 0;
    data->hit = 0;
    data->mapX = 0;
    data->mapY = 0;
    data->planeX = 0;
    data->planeY = 0;
    data->posx = 0;
    data->posy = 0;
    data->prepwalldist = 0;
    data->raydirX = 0;
    data->raydirY = 0;
    data->side = 0;
    data->sidedistX = 0;
    data->sidedistY = 0;
    data->stepx = 0;
    data->stepy = 0;
    data->compass = 0;
    data->player_i = 0;
    data->player_j = 0;
    data->map = map;
    data->wall = wall;
}

void wall_init(t_data *data)
{
    data->wall->draw_end = 0;
    data->wall->draw_start = 0;
    data->wall->line_length = 0; 
}

void set_dda_vars(t_data *data)
{
    data->deltadisX = fabs(1/data->raydirX);
    data->deltadisY = fabs(1/data->raydirY);
    if (data->raydirX < 0)
    {
    	data->stepx = -1;
    	data->sidedistX = (data->posx - ((float)data->mapX)) * data->deltadisX;
        // printf("posx is %f, mapX is %d, delta is %f\n", data->posx, data->mapX, data->deltadisX);
    }
    else
    {
        data->stepx = 1;
        data->sidedistX = (((float)data->mapX) + 1.0 - data->posx) * data->deltadisX;
    }
    if (data->raydirY < 0)
    {
        data->stepy = -1;
        data->sidedistY = (data->posy - ((float)data->mapY)) * data->deltadisY;
    }
    else
    {
    	data->stepy = 1;
        data->sidedistY = (((float)data->mapY) + 1.0 - data->posy) * data->deltadisY;
    }
}

void dda(t_data *data)
{
    while (data->hit != 1)
    {
        if (data->sidedistX < data->sidedistY)
        {
            data->sidedistX += data->deltadisX;
            //printf("sidedistx is %f deltadistx is %f\n",data->sidedistX, data->deltadisX);
            data->mapX += data->stepx;
            data->side = 0;
        }
        else
        {
          data->sidedistY += data->deltadisY;
            //printf("mapY is %d, sidedisty is %f deltadisty is %f\n",data->mapY, data->sidedistY, data->deltadisY);
          data->mapY += data->stepy;
          data->side = 1;
        }
        if(data->mapX < 0 || data->mapX > data->map->map_j - 1 || data->mapY < 0 || data->mapY > data->map->map_i - 1 )
        {
            //printf("no wall :( \n");
            break;
            // return 1;
        }
        //printf("mapx is %d but map_j iiiis %d\n", data->mapX , data->map->map_j);
        //printf("mapY is %d but map_i iiiis %d\n", data->mapY , data->map->map_i);
        if (data->map->map[data->mapY][data->mapX] == '1')
        {
            //printf("hit a wall x %d and y %d\n", data->mapX, data->mapY);
            data->hit = 1;
        }
    }
    
}

void calc_length(t_data *data)
{
    if (data->side == 0)
    {
        data->prepwalldist = data->sidedistX - data->deltadisX;
        //printf("prepwalldist is %f sidedistx is %f deltadist is %f\n", data->prepwalldist, data->sidedistX, data->deltadisX);
    }
    else
    {
        data->prepwalldist = data->sidedistY - data->deltadisY;
        //printf("prepwalldist is %f sidedistx is %f deltadist is %f\n", data->prepwalldist, data->sidedistY, data->deltadisY);
    }
    //if ((int)data->prepwalldist != 0)
    //if ((int)data->prepwalldist == 0)
    //{
    //    printf("errooooooor\n");
    //    return;    
    // }
    if (data->prepwalldist == 0)
    {
        // printf("og prep is %f\n", data->prepwalldist);
        data->wall->line_length = ((float)screenHeight / 0.1);
    }
    else
        data->wall->line_length = ((float)screenHeight / (data->prepwalldist));
	// else
	// 	data->wall->line_length = screenHeight;
    data->wall->draw_start = ((-data->wall->line_length) / 2) + ((float)screenHeight / 2);
    if(data->wall->draw_start < 0)
        data->wall->draw_start = 0;
    // printf("line_length -> %d, perp %f\n", data->wall->line_length, data->prepwalldist);
    // exit(1);
    data->wall->draw_end = ((data->wall->line_length) / 2) + ((float)screenHeight / 2);
    if (data->wall->draw_end > (float)screenHeight)
        data->wall->draw_end = (float)screenHeight - 1;
}

void ray_init(t_data *data)
{
    data->cameraX = 0;
    data->deltadisX = 0;
    data->deltadisY = 0;
    data->hit = 0;
    data->prepwalldist = 0;
    data->raydirX = 0;
    data->raydirY = 0;
    data->side = 0;
    data->sidedistX = 0;
    data->sidedistY = 0;
    data->stepx = 0;
    data->stepy = 0;
    data->mapX = (int)(data->posx);
    data->mapY = (int)data->posy;
}

int rgb_c(t_data *data)
{
    // printf("r = %d | g = %d | b = %d  \n", data->map->c_color[0], data->map->c_color[1] , data->map->c_color[2]);
    return((data->map->c_color[0] << 16) | (data->map->c_color[1] << 8) | data->map->c_color[2]);
}

int rgb_f(t_data *data)
{
    // printf("r = %d | g = %d | b = %d  \n", data->map->c_color[0], data->map->c_color[1] , data->map->c_color[2]);
    return((data->map->f_color[0] << 16) | (data->map->f_color[1] << 8) | data->map->f_color[2]);
}

void load_tex(t_data *data)
{

    if (data->img == NULL)
    {
        return;
    }
    
    
        data->img->img_w = mlx_xpm_file_to_image(data->mlx, "./textures/tex4.xpm", &data->img->win_width, &data->img->win_height);
   
        data->img->img_e = mlx_xpm_file_to_image(data->mlx,"./textures/tex4.xpm", &data->img->win_width, &data->img->win_height);
   
        data->img->img_n = mlx_xpm_file_to_image(data->mlx, "./textures/tex4.xpm", &data->img->win_width, &data->img->win_height);
   
        data->img->img_s = mlx_xpm_file_to_image(data->mlx, "./textures/tex4.xpm", &data->img->win_width, &data->img->win_height);
}

void init_img(t_data *data)
{
    data->img->add_pxl_e = mlx_get_data_addr(data->img->img_e, &data->img->bits_per_pixel_e, &data->img->line_length_e, &data->img->endian_e);
    data->img->add_pxl_w = mlx_get_data_addr(data->img->img_w, &data->img->bits_per_pixel_w, &data->img->line_length_w, &data->img->endian_w);
    data->img->add_pxl_s = mlx_get_data_addr(data->img->img_s, &data->img->bits_per_pixel_s, &data->img->line_length_s, &data->img->endian_s);
    data->img->add_pxl_n = mlx_get_data_addr(data->img->img_n, &data->img->bits_per_pixel_n, &data->img->line_length_n, &data->img->endian_n);
}

void draw_fc(t_data *data, int x, double *y)
{
    int i = 0;
    
    while(i < data->wall->draw_start)
    {
        my_mlx_pixel_put(data->img, x, i, rgb_c(data));
        i++;
        (*y)++;
    }
    i = data->wall->draw_end;
    while(i < screenHeight)
    {
        my_mlx_pixel_put(data->img, x, i, rgb_f(data));
        i++;
    }
}

void draw_w(t_data *data, int x, int y, int tex_y)
{
    int color;
    double tex_x;
    double wall_x;

    if (data->side == 0) 
        wall_x = data->posy + data->prepwalldist * data->raydirY;
    else // Vertical wall
        wall_x = data->posx + data->prepwalldist * data->raydirX;

    wall_x -= floor(wall_x); 
    tex_x = (int)(wall_x * (double)data->img->win_width);

    if (data->side == 0 && data->raydirX > 0) 
        tex_x = data->img->win_width - tex_x - 1;
    if (data->side == 1 && data->raydirY < 0) 
        tex_x = data->img->win_width - tex_x - 1;

    if (tex_y >= 0 && tex_y < data->img->win_height && tex_x >= 0 && tex_x < data->img->win_width)
    {
        color = *(int *)(data->img->add_pxl_w + ((int)tex_y * data->img->line_length_w + (int)tex_x * (data->img->bits_per_pixel_w / 8)));
        my_mlx_pixel_put(data->img, x, y, color);
    }
}

void draw_s(t_data *data, double x, double y, double tex_y)
{
    int color;
    double tex_x;
    double wall_x;

    if (data->side == 0) // Horizontal wall
        wall_x = data->posy + data->prepwalldist * data->raydirY;
    else // Vertical wall
        wall_x = data->posx + data->prepwalldist * data->raydirX;

    wall_x -= floor(wall_x); 
    tex_x = (int)(wall_x * (double)data->img->win_width);

    
    if (data->side == 0 && data->raydirX > 0) 
        tex_x = data->img->win_width - tex_x - 1;
    if (data->side == 1 && data->raydirY < 0) 
        tex_x = data->img->win_width - tex_x - 1;

    if (tex_y >= 0 && tex_y < data->img->win_height && tex_x >= 0 && tex_x < data->img->win_width)
    {
        color = *(int *)(data->img->add_pxl_s + ((int)tex_y * data->img->line_length_s + (int)tex_x * (data->img->bits_per_pixel_s / 8)));
        my_mlx_pixel_put(data->img, x, y, color);
    }
}

void draw_e(t_data *data, double x, double y, double tex_y)
{
    int color;
    double tex_x;
    double wall_x;

    if (data->side == 0) // Horizontal wall
        wall_x = data->posy + data->prepwalldist * data->raydirY;
    else // Vertical wall
        wall_x = data->posx + data->prepwalldist * data->raydirX;

    wall_x -= floor(wall_x); 
    tex_x = (int)(wall_x * (double)data->img->win_width);

  
    if (data->side == 0 && data->raydirX > 0) 
        tex_x = data->img->win_width - tex_x - 1;
    if (data->side == 1 && data->raydirY < 0) 
        tex_x = data->img->win_width - tex_x - 1;

    if (tex_y >= 0 && tex_y < data->img->win_height && tex_x >= 0 && tex_x < data->img->win_width)
    {
        color = *(int *)(data->img->add_pxl_e + ((int)tex_y * data->img->line_length_e + (int)tex_x * (data->img->bits_per_pixel_e / 8)));
        my_mlx_pixel_put(data->img, x, y, color);
    }
}

void draw_n(t_data *data, double x, double y, double tex_y)
{
    int color;
    double tex_x;
    double wall_x;

    if (data->side == 0) // Horizontal wall
        wall_x = data->posy + data->prepwalldist * data->raydirY;
    else // Vertical wall
        wall_x = data->posx + data->prepwalldist * data->raydirX;

    wall_x -= floor(wall_x); 
    tex_x = (int)(wall_x * (double)data->img->win_width);

    
    if (data->side == 0 && data->raydirX > 0) 
        tex_x = data->img->win_width - tex_x - 1;
    if (data->side == 1 && data->raydirY < 0) 
        tex_x = data->img->win_width - tex_x - 1;

    if (tex_y >= 0 && tex_y < data->img->win_height && tex_x >= 0 && tex_x < data->img->win_width)
    {
        color = *(int *)(data->img->add_pxl_n + ((int)tex_y * data->img->line_length_n + (int)tex_x * (data->img->bits_per_pixel_n / 8)));
        my_mlx_pixel_put(data->img, x, y, color);
    }
}

void render_wall(t_data *data, int x)
{
    double distance;
   double off_y;
    double y;
    // int color = 0;

    y = 0;
    draw_fc(data, x, &y);
    init_img(data);
    // printf("Texture Width: %d, Height: %d\n", data->img->win_width, data->img->win_height);
//printf("prepwalldist: %f, line_length: %f\n", data->prepwalldist, data->wall->line_length);
    
    // printf("end = %f\n", data->wall->draw_end);
    // printf("wall = %f\n", data->wall->line_length);
    // printf("y = %f\n", y);
    while(y <= data->wall->draw_end)
    {
       distance = y + ((data->wall->line_length / 2) -  (screenHeight / 2));
       off_y = distance * ((double)data->img->win_height / data->wall->line_length);

        if(data->side == 0 && data->stepx == -1)
        {
            // color = 0xFF0000; // RED
            // my_mlx_pixel_put(data->img, x, y, color);
            draw_w(data, x, y, (int)off_y);
        }
        else if(data->side == 0 && data->stepx == 1)
        {
            // color = 0xFFFF00; // Yellow
            // my_mlx_pixel_put(data->img, x, y, color);
            //printf("----%f\n", off_y);
          draw_e(data, x, y, (int)off_y);
        }
        else if(data->side == 1 && data->stepy == -1)
        {
            // color = 0x0000FF; // Blue
            // my_mlx_pixel_put(data->img, x, y, color);
           draw_n(data, x, y, (int)off_y);
        }
        else if(data->side == 1 && data->stepy == 1)
        {
            // color = 0xFFB6C1;// pink
            // my_mlx_pixel_put(data->img, x, y, color);
            draw_s(data, x, y, (int)off_y);
        }
        y++;
    }
    // printf("***************\n");

}

void raycast(t_data *data)
{
    int x;

    x = 0;
	create_image(data);
    load_tex(data);
    while(x < screenWidth)
    {
        wall_init(data);
        ray_init(data);
        data->cameraX = 2 * x / (float)screenWidth - 1;
		//printf("camerax is %f\n", data->cameraX);
        data->raydirX = data->dirx + (data->planeX * data->cameraX);
        //printf("raydirx is %f\n", data->raydirX); 
        data->raydirY = data->diry + (data->planeY * data->cameraX);
        //printf("raydiry is %f\n", data->raydirY);
 
        set_dda_vars(data);
        dda(data);
        if(data->hit == 1)
        {
            calc_length(data);
            // draw_a_line(data, x);
            // draw_fc(data, x);
            render_wall(data, x);


        }
        data->hit = 0;
        x++;
    }
    draw_minimap(data, data->minimap);
    
}

void player_init(t_data *data)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (data->map->map[i])
    {
        while(data->map->map[i][j])
        {
            if (data->map->map[i][j] == 'N' || data->map->map[i][j] == 'E'
                || data->map->map[i][j] == 'S'|| data->map->map[i][j] == 'W')
            {
                data->compass = data->map->map[i][j];
                data->player_i = i;
                data->player_j = j;
                data->posx = ((float)j) + 0.5;
				//printf("posx is %f\n", data->posx);
                data->posy = ((float)i) + 0.5;

            }
            j++;
        }
        j = 0;
        i++;
    }
}
