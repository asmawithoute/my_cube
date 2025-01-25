/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 14:20:48 by meabdelk          #+#    #+#             */
/*   Updated: 2025/01/25 16:50:40 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void file_err(int i)
{
    if(i == 1)
    {
        printf("Error ! file \n");
        exit(1);
    }
    if(i == 2)
    {
        printf("Error ! opening file !\n");
        exit( 0);
    }
    if(i == 3)
    {
        printf("Error ! empty map \n");
        exit(1);
    }
}

void check_file(char *file)
{
    int i;
    int j;
    char *str;

    i = ft_strlen(file);
    str = ".cub";
    j = ft_strlen(str);
    if(i <= j)
        file_err(1);
    while(j >= 0)
    {
        if(file[i] != str[j])
        {
           file_err(1);
        }
        i--;
        j--;
    }
    if(i >= 0 && file[i] == '/')
        file_err(1);
}

int ft_countlines(char *av)
{
    int count;
    char *line;
    int fd;

    count = 0;
    fd = open(av, O_RDONLY);
    if(fd == -1)
        file_err(2);
    line = get_next_line(fd);
    while(line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    if(count == 0)
    {
        printf("error ! empty file \n");
        exit(0);
    }
    return(count);
}


//put lines in char**map and the rest in textures and floor/ceiling
void get_map(char *av, t_map *map)
{
    int fd;
    char *line;
    int i;
    
    i = 0;
    fd = open(av, O_RDONLY);
    if(fd == -1)
        file_err(2);
    map->countlines = ft_countlines(av);
    if(map->countlines == 0)
        file_err(3);
    map->line = malloc(sizeof(char *) * (map->countlines + 1));
    if(!map->line)
        exit(1);
    line = get_next_line(fd);
    if(line[0] == '\n')
    {
        printf("ERROR new_line first row \n");
        free(line);
        exit(1);
    }
    while(line)
    {
        map->line[i] = line;
        i++;
        line = get_next_line(fd);
    }
    map->line[map->countlines] = NULL;
    free(line);
    close(fd);
}

void ini(t_map *map)
{
    map->count->no_count = 0;
    map->count->so_count = 0;
    map->count->we_count = 0;
    map->count-> ea_count = 0;
    map->count->c_count = 0;
    map->count->f_count = 0;
}

void check_multiple(t_map *map)
{
    int i;

    i = 0;
    ini(map);
    while(map->line[i])
    {
        if(ft_strncmp(map->line[i], "NO", 2) == 0 && (map->line[i][2] == ' '))
           map->count-> no_count++;
        else if(ft_strncmp(map->line[i], "SO", 2) == 0 && (map->line[i][2] == ' '))
            map->count->so_count++;
        else if(ft_strncmp(map->line[i], "WE", 2) == 0 && (map->line[i][2] == ' '))
            map->count->we_count++;
        else if(ft_strncmp(map->line[i], "EA", 2) == 0 && (map->line[i][2] == ' '))
            map->count->ea_count++;
        else if(ft_strncmp(map->line[i], "F", 1) == 0 && (map->line[i][1] == ' '))
            map->count->f_count++;
        else if(ft_strncmp(map->line[i], "C", 1) == 0 && (map->line[i][1] == ' '))
            map->count->c_count++;
        i++;
    }
    if(map->count->no_count != 1 || map->count->so_count != 1 || map->count->we_count != 1 
        || map->count->ea_count != 1 || map->count->f_count != 1 || map->count->c_count != 1)
    {
        printf("Error multiple textures !! \n");
        free(map->count);
        exit(1);
    }
}


void check_textures(t_map *map, int *i)
{
    int count;

    count = 0;
    while(map->line[*i] && *i < 4)
    {
        if((ft_strncmp(map->line[*i], "NO", 2) == 0 
            || ft_strncmp(map->line[*i], "SO", 2) == 0
            || ft_strncmp(map->line[*i], "WE", 2) == 0
            || ft_strncmp(map->line[*i], "EA", 2) == 0 ) && (map->line[*i][2] == ' '))
        {
            check_multiple(map);
            count++;
        }
        (*i)++;
    }
    if(count != 4)
        printf(" ERROR texture \n");
}

void check_fc(t_map *map, int *i)
{
    int count;

    count = 0;
    (*i)++;
    while(map->line[*i] && count < 2)
    {
        
        printf(" --> line = %s\n", map->line[*i]);
        if((ft_strncmp(map->line[*i], "F", 1) || ft_strncmp(map->line[*i], "C", 1))
            && (map->line[*i][1] == ' '))
        {
            count++;        
        }
        (*i)++;
    }
    if(count != 2)
        printf(" ERROR floor and ceiling \n");
}

int get_longest_line(char **line)
{
	int i;
	int j;
	int longest;

	i = 0;
	j = 0;
	longest = 0;
	while(line[i])
	{
		while(line[i][j])
		{
			j++;
		}
		if(j > longest)
			longest = j;
		j = 0;
		i++;
	}
	return (longest);
}

void parse_map(t_map *map, int i)
{
	int j;
	int size;

	j = i;
	size = 0;
	int l = get_longest_line(map->line);
    map->map_j = l;
	while (map->line[j])
	{
		size++;
		j++;
	}
    map->map_i = size;
	printf("size is is %d\n", size);
	map->map = malloc(sizeof(char *)* (size + 1));
	j = 0;
	while (j < size)
	{
		map->map[j] = malloc(sizeof(char) * (l + 1));
		j++;
	}
	j = 0;
	l = 0;
	while (map->line[i])
	{
		while (map->line[i][j])
		{
			map->map[l][j] = map->line[i][j];
			j++;
		}
		map->map[l][j] = '\0';
		// printf("l is %d map is %s\n",l,  map->map[l]);
		i++;
		l++;
		j = 0;
	}
	map->map[size] = NULL;
}

void check_map(t_map *map)
{
    int i;

    i = 0;
    check_textures(map, &i);
    // printf("first line = %s\n", map->line[i]);
    while (map->line[i] && ft_strchar(map->line[i], '\n') == 0)
    {
        i++;
    }
    check_fc(map, &i);
    while (map->line[i] && ft_strchar(map->line[i], '\n') == 0)
    {
        i++;
    }
	parse_map(map, i + 1);
    // printf("after line = %s\n", map->line[i + 1]);
    
}

void init_data(t_map *map)
{
    map->countlines = 0;
    map->line = NULL;
    map->count = malloc(sizeof(t_count));
    map->map_i = 0;
    map->map_j = 0;
    if(!map->count)
        return;
}

void create_image(t_data *data)
{
	t_mlx *img;

    img = malloc(sizeof(t_mlx));

    img->img = mlx_new_image(data->mlx, screenWidth, screenHeight);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length,
								&img->endian);
    data->img = img;
}

int left(t_data *data)
{
    printf("\n\n posx before %f\n", data->posx);
    if(data->posx - 0.5 <= 0)
    {
        printf("rorororo\n");
        return 1;
    }
    data->posx -= 0.5;
    printf("\n\n posx after %f\n", data->posx);
    raycast(data);
    mlx_clear_window(data->mlx, data->mlx_win);
    mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0); 
    return 0;
}

int right(t_data *data)
{
    if (data->posx + 0.5 >= (float)data->map->map_j)
        return 1;
    data->posx += 0.5;
    raycast(data);
    mlx_clear_window(data->mlx, data->mlx_win);
    mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
    return 0;
}

void move_mini_player_front(t_data *data)
{
    if (data->compass == 'N')
    {
        data->map->map[data->player_i - 1][data->player_j] = 'N';
        data->map->map[data->player_i][data->player_j] = '0';
        data->player_i--;
    }
}

int front(t_data *data)
{
    // data->posx += data->dirx;
    // data->posy += data->diry;
    if (data->posy - 0.5 <= 0)
        return 1;
    data->posy -= 0.5;
    move_mini_player_front(data);
    raycast(data);
    mlx_clear_window(data->mlx, data->mlx_win);
    mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0); 
    return 0;
//     raycast(data);
//     mlx_clear_window(data->mlx, data->mlx_win);
//     mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);    
}

int back(t_data *data)
{
    // data->posx -= data->dirx;
    // data->posy -= data->diry;
    if (data->posy + 0.5 >= (float)data->map->map_i)
        return 1;
    data->posy += 0.5;
    raycast(data);
    mlx_clear_window(data->mlx, data->mlx_win);
    mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0); 
    return 0;
    // raycast(data);
    // mlx_clear_window(data->mlx, data->mlx_win);
    // mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);    
}

int ray(int keycode ,t_data *data)
{
    // raycast(data);
    // (int)data;
    // printf("key is %d\n", keycode);
    // mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
    // mlx_clear_window(data->mlx, data->mlx_win);
    if (keycode == 119)
		if(front(data) == 1)
            return 0;
	if (keycode == 115)
		if (back(data) == 1)
            return 0;
	if (keycode == 97)
		if (left(data) == 1)
            return 0;
	if (keycode == 100)
		if (right(data) == 1)
            return 0;
	if (keycode == 65307)
		exit(1);
	return 0;
}

void    create_window(t_data *data, t_map *map, t_wall *wall)
{
    // void	*mlx;
	// void	*mlx_win;
	// t_map   map;
    // printf("map --> %s\n", map->map[0]);
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, screenWidth, screenHeight, "Hello world!");
	// draw_a_line(&img);
    ray_data_init(data, map, wall);
    player_init(data);
    data_init(data);
    mini_map(data);
    raycast(data);
    mlx_clear_window(data->mlx, data->mlx_win);
    mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
    mlx_key_hook(data->mlx_win, &ray, data);
	mlx_loop(data->mlx);
}

int main(int ac, char **av)
{
    t_map *map;
    t_wall wall;
    t_data data;
 
    if(ac <= 1)
        exit(0);
    if(ac != 2)
    {
        printf("Error\nnumber of args\n");
	    exit(0);
    }
    map = malloc(sizeof(t_map));
    check_file(av[1]);
    init_data(map);
    get_map(av[1], map);
    check_map(map);
    create_window(&data, map, &wall);
    free_all(map);
    return(0);
}
