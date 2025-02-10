/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoraich <akoraich@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 14:20:48 by meabdelk          #+#    #+#             */
/*   Updated: 2025/02/10 17:16:34 by akoraich         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	file_err(int i)
{
	if (i == 1)
	{
		printf("Error ! file \n");
		exit(1);
	}
	if (i == 2)
	{
		printf("Error ! opening file !\n");
		exit(0);
	}
	if (i == 3)
	{
		printf("Error ! empty map \n");
		exit(1);
	}
}

void	check_file(char *file)
{
	int		i;
	int		j;
	char	*str;

	i = ft_strlen(file);
	str = ".cub";
	j = ft_strlen(str);
	if (i <= j)
		file_err(1);
	while (j >= 0)
	{
		if (file[i] != str[j])
		{
			file_err(1);
		}
		i--;
		j--;
	}
	if (i >= 0 && file[i] == '/')
		file_err(1);
}

int	ft_countlines(char *av)
{
	int		count;
	char	*line;
	int		fd;

	count = 0;
	fd = open(av, O_RDONLY);
	if (fd == -1)
		file_err(2);
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (count == 0)
	{
		printf("error ! empty file \n");
		exit(0);
	}
	return (count);
}

// put lines in char**map and the rest in textures and floor/ceiling
void	get_map(char *av, t_map *map)
{
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open(av, O_RDONLY);
	if (fd == -1)
		file_err(2);
	map->countlines = ft_countlines(av);
	if (map->countlines == 0)
		file_err(3);
	map->line = malloc(sizeof(char *) * (map->countlines + 1));
	if (!map->line)
		exit(1);
	line = get_next_line(fd);
	// if(line[0] == '\n')
	// {
	//     printf("ERROR new_line first row \n");
	//     free(line);
	//     exit(1);
	// }
	while (line)
	{
		map->line[i] = line;
		i++;
		line = get_next_line(fd);
	}
	map->line[map->countlines] = NULL;
	free(line);
	close(fd);
}

void	ini(t_map *map)
{
	map->count->no_count = 0;
	map->count->so_count = 0;
	map->count->we_count = 0;
	map->count->ea_count = 0;
	map->count->c_count = 0;
	map->count->f_count = 0;
}

int	skp_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
	{
		i++;
	}
	return (i);
}

int	check_path(const char *path)
{
	int	fd;

	// printf("path : %s\n", path);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("Error ! invalid texture path : %s \n", path);
		return (1);
	}
	close(fd);
	return (0);
}

int	count_part(char **value)
{
	int	i;

	if (!value)
		return (0);
	i = 0;
	while (value[i])
	{
		i++;
	}
	return (i);
}
void	free_text(char **texture)
{
	int	i;

	if (!texture)
		return ;
	i = 0;
	while (texture[i])
	{
		free(texture[i]);
		i++;
	}
	free(texture);
}

static int	ft_check(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strlen2(const char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*p;
	int		i;
	int		l;
	int		j;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	if (!s1[i])
		return (ft_strdup(""));
	while (ft_check(set, s1[i]))
		i++;
	l = ft_strlen2(s1);
	while (ft_check(set, s1[l - 1]))
		l--;
	p = (char *)malloc(l - i + 1);
	if (!p)
		return (0);
	j = 0;
	while (i < l)
		p[j++] = s1[i++];
	p[j] = '\0';
	return (p);
}

void	check_valid(char *line, char **path)
{
	char	**value;
	char	*temp;

	value = ft_split(line, ' ');
	if (!value || count_part(value) != 2)
	{
		printf("Error ! invalid texture \n");
		free_text(value);
		exit(0);
	}
	temp = ft_strdup(value[1]);
	*path = ft_strtrim(temp, "\n");
	free(temp);
	if (!*path)
	{
		free_text(value);
		exit(0);
	}
	free_text(value);
	if (check_path(*path) != 0)
	{
		//    free_text(path);
		exit(0);
	}
}

int	count_comma(char *value)
{
	int		j;
	int		count;
	char	*str;

	if (!value)
		return (0);
	count = 0;
	j = 0;
	str = ft_strtrim(value, "\n");
	while (str[j])
	{
		if (str[j] == ',')
		{
			count++;
		}
		j++;
	}
	free(str);
	return (count);
}

char	**check_format_color(char *line, int flag)
{
	char	**value;
	char	**color_parts;

	if (count_comma(line) != 2)
		file_err(4);
	if (flag == 0)
		value = ft_split(line, 'F');
	else
		value = ft_split(line, 'C');
	if (!value || count_part(value) < 2)
	{
		printf("Error! invalid format\n");
		free_text(value);
		exit(0);
	}
	color_parts = ft_split(value[1], ',');
	free_text(value);
	if (!color_parts)
	{
		printf("Error! invalid color format \n");
		free_text(color_parts);
		exit(0);
	}
	return (color_parts);
}

int	check_digit(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] >= '0' && value[i] <= '9')
			i++;
		else
			return (1);
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	check_range(int range)
{
	if (range < 0 || range > 255)
	{
		printf("error ! invalid color range \n");
		return (1);
	}
	return (0);
}

void	check_valid_color(char *line, int *color, int flag)
{
	char	**value;
	int		i;
	int		j;

	value = check_format_color(line, flag);
	i = 0;
	j = 0;
	while (i < 3)
	{
		value[i] = ft_strtrim(value[i], " \n");
		if (value[i][j] == '+')
			j++;
		skp_spaces(value[i]);
		if (check_digit(&value[i][j]) != 0)
		{
			printf("Error: Color values must be digits\n");
			free_text(value);
			exit(0);
		}
		color[i] = ft_atoi(value[i]);
		if (check_range(color[i]) != 0)
		{
			free_text(value);
			exit(0);
		}
		i++;
	}
	free_text(value);
}

void	f_c_check(t_map *map, int *i, int j)
{
	if (ft_strncmp(&map->line[*i][j], "F", 1) == 0 && (map->line[*i][j
			+ 1] == ' '))
	{
		check_valid_color(map->line[*i], map->f_color, 0);
		map->count->f_count++;
	}
	else if (ft_strncmp(&map->line[*i][j], "C", 1) == 0 && (map->line[*i][j
			+ 1] == ' '))
	{
		check_valid_color(map->line[*i], map->c_color, 1);
		map->count->c_count++;
	}
}

void	process_line(t_map *map, int *i, int j)
{
	if (ft_strncmp(&map->line[*i][j], "NO", 2) == 0 && (map->line[*i][j
			+ 2] == ' '))
	{
		check_valid(map->line[*i], &map->no_texture);
		map->count->no_count++;
	}
	else if (ft_strncmp(&map->line[*i][j], "SO", 2) == 0 && (map->line[*i][j
			+ 2] == ' '))
	{
		check_valid(map->line[*i], &map->so_texture);
		map->count->so_count++;
	}
	else if (ft_strncmp(&map->line[*i][j], "WE", 2) == 0 && (map->line[*i][j
			+ 2] == ' '))
	{
		check_valid(map->line[*i], &map->we_texture);
		map->count->we_count++;
	}
	else if (ft_strncmp(&map->line[*i][j], "EA", 2) == 0 && (map->line[*i][j
			+ 2] == ' '))
	{
		check_valid(map->line[*i], &map->ea_texture);
		map->count->ea_count++;
	}
	else if (ft_strncmp(&map->line[*i][j], "F", 1) == 0
		|| ft_strncmp(&map->line[*i][j], "C", 1) == 0)
		f_c_check(map, i, j);
	(*i)++;
}

void	check_multiple(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	ini(map);
	while (map->line[i])
	{
		j = skp_spaces(map->line[i]);
		process_line(map, &i, j);
	}
	if (map->count->no_count != 1 || map->count->so_count != 1
		|| map->count->we_count != 1 || map->count->ea_count != 1
		|| map->count->f_count != 1 || map->count->c_count != 1)
	{
		printf("Error textures !! \n");
		free(map->count);
		exit(0);
	}
}

// void free_map(t_map *map)
// {
//     int i;

//     i = 0;
//     while (i < map->countlines)
//     {
//         free(map->line[i]);
//         i++;
//     }
//     free(map->line);
// }

void	freee(t_map *map)
{
	free_map(map);
	free(map->count);
}

void	check_textures(t_map *map, int i)
{
	int	count;
	int	j;

	count = 0;
	j = 0;
	while (map->line[i])
	{
		j = skp_spaces(map->line[i]);
		if ((ft_strncmp(&map->line[i][j], "NO", 2) == 0
				|| ft_strncmp(&map->line[i][j], "SO", 2) == 0
				|| ft_strncmp(&map->line[i][j], "WE", 2) == 0
				|| ft_strncmp(&map->line[i][j], "EA", 2) == 0)
			&& (map->line[i][j + 2] == ' '))
		{
			count++;
		}
		(i)++;
	}
	// printf("count == %d\n", count);
	if (count != 4)
	{
		printf(" ERROR texture \n");
		freee(map);
		exit(0);
	}
}

void	check_fc(t_map *map, int i)
{
	int	count;
	int	j;

	count = 0;
	while (map->line[i] && count < 2)
	{
		j = skp_spaces(map->line[i]);
		if ((ft_strncmp(&map->line[i][j], "F", 1)
				|| ft_strncmp(&map->line[i][j], "C", 1)) && (map->line[i][j
				+ 1] == ' '))
		{
			count++;
		}
		(i)++;
	}
	// printf("count fc == %d\n", count);
	if (count != 2)
	{
		printf(" ERROR floor and ceiling \n");
		freee(map);
		exit(0);
	}
}

void	ft_trim(char *str)
{
	int	len;

	len = ft_strlen(str) - 1;
	while (len >= 0 && (str[len] == ' ' || str[len] == '\t'
			|| str[len] == '\n'))
	{
		str[len] = '\0';
		len--;
	}
}
int	get_longest_line(char **line, int i)
{
	int	j;
	int	longest;

	// printf("heeeeeeeeere\n");
	// int i;/
	// i = 0;
	j = 0;
	longest = 0;
	while (line[i])
	{
		ft_trim(line[i]);
		while (line[i][j] != '\0' && line[i][j] != '\n')
		{
			j++;
		}
		if (j > longest)
			longest = j;
		j = 0;
		i++;
	}
	return (longest);
}

// int get_longest_line(char **line, int i)
// {
//     // printf("heeeeeeeeere\n");
// 	// int i;/
// 	int j;
// 	int longest;

// 	// i = 0;
// 	j = 0;
// 	longest = 0;
// 	while(line[i])
// 	{
// 		while(line[i][j] != '\0' && line[i][j] != '\n')
// 		{
// 			j++;
// 		}
// 		if(j > longest)
// 			longest = j;
// 		j = 0;
// 		i++;
// 	}
// 	return (longest);
// }

// void parse_map(t_map *map, int i)
// {
// 	int j;
// 	int size;

// 	j = i;
// 	size = 0;
//     // printf("here %s\n", data->map->map[0]);
// 	int l = get_longest_line(map->line, i);
//     // int l = map->countlines_map;
//     // printf("after %d\n", l);
//     // exit(0);
//     map->map_j = l;
// 	while (map->line[j])
// 	{
// 		size++;
// 		j++;
// 	}
//     map->map_i = size;
// 	// printf("size is is %d\n", size);
// 	map->map = malloc(sizeof(char *) * (size + 1));
// 	j = 0;
// 	while (j < size)
// 	{
// 		map->map[j] = malloc(sizeof(char) * (l + 1));
// 		j++;
// 	}
// 	j = 0;
// 	l = 0;
// 	while (map->line[i])
// 	{
// 		while (map->line[i][j] != '\0' && map->line[i][j] != '\n')
// 		{
// 			map->map[l][j] = map->line[i][j];
// 			j++;
// 		}
// 		while (j < map->map_j)
// 		{
// 			map->map[l][j] = ' ';
// 			j++;
// 		}
// 		map->map[l][j] = '\0';
// 		// printf("j %d\n", j);
// 		// printf("l is %d map is %s\n",l,  map->map[l]);
// 		i++;
// 		l++;
// 		j = 0;
// 	}
// 	map->map[size] = NULL;
// }

void	parse_map(t_map *map, int i)
{
	int	j;
	int	size;
	int	l;

	j = i;
	size = 0;
	// printf("here %s\n", data->map->map[0]);
	l = get_longest_line(map->line, i);
	// int l = map->countlines_map;
	// printf("after %d\n", l);
	// exit(0);
	map->map_j = l;
	while (map->line[j])
	{
		size++;
		j++;
	}
	map->map_i = size;
	// printf("size is is %d\n", size);
	map->map = malloc(sizeof(char *) * (size + 1));
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
		ft_trim(map->line[i]);
		while (map->line[i][j] != '\0' && map->line[i][j] != '\n')
		{
			map->map[l][j] = map->line[i][j];
			j++;
		}
		while (j < map->map_j)
		{
			map->map[l][j] = ' ';
			j++;
		}
		map->map[l][j] = '\0';
		// printf("j %d\n", j);
		// printf("l is %d map is %s\n",l,  map->map[l]);
		i++;
		l++;
		j = 0;
	}
	map->map[size] = NULL;
}

int	ft_find_map(t_map *map, int *i)
{
	int	j;

	while (map->line[*i])
	{
		j = skp_spaces(map->line[*i]);
		if (map->line[*i][j] == '1')
		{
			return (0);
		}
		(*i)++;
	}
	return (1);
}

void	check_last(t_map *map, int *i)
{
	int	k;
	int	j;
	int	len;

	k = *i;
	j = 0;
	while (map->line[k])
	{
		k++;
	}
	map->countlines_map = k - (*i);
	len = ft_strlen(map->line[k - 1]) - 1;
	while (map->line[k - 1][j] && j < len)
	{
		if (map->line[k - 1][j] != '1' && map->line[k - 1][j] != ' '
			&& map->line[k - 1][j] != '\t')
		{
			printf("Error \n invalid border \n ");
			free_all(map);
			exit(0);
		}
		j++;
	}
}

void	check_first_last(t_map *map, int *i)
{
	int	k;
	int	j;
	int	len;

	k = *i;
	j = 0;
	len = ft_strlen(map->line[k]) - 1;
	while (map->line[k][j] && j < len)
	{
		if (map->line[k][j] != '1' && map->line[k][j] != ' '
			&& map->line[k][j] != '\t')
		{
			printf("Error \n invalid border\n ");
			freee(map);
			exit(0);
		}
		j++;
	}
	check_last(map, i);
}

void	check_map_borders(t_map *map, int *i)
{
	int	k;
	int	j;

	k = *i;
	while (map->line[k])
	{
		j = skp_spaces(map->line[k]);
		if (map->line[k][0] == '\n')
		{
			k++;
			continue ;
		}
		if (map->line[k][j] != '1' || (map->line[k][ft_strlen(map->line[k])
				- 2] != '1' && map->line[k][ft_strlen(map->line[k]) - 2] != ' '
				&& map->line[k][ft_strlen(map->line[k]) - 2] != '\t'))
		{
			printf("Error\n Invalid map !\n");
			printf(" -------->k == %d \n", k);
			freee(map);
			exit(0);
		}
		k++;
	}
}

void	ft_error(t_map *map)
{
	int	count;

	count = map->e + map->w + map->n + map->s;
	if (count == 0)
	{
		printf("error \n missing player !\n");
		freee(map);
		exit(0);
	}
	else if (count != 1)
	{
		printf("error \n more than one player found!\n");
		freee(map);
		exit(0);
	}
}

void	check_characters(t_map *map, int *i)
{
	int	k;
	int	j;

	k = *i;
	while (map->line[k])
	{
		j = ft_strlen(map->line[k]) - 2;
		while (j >= 0)
		{
			if (map->line[k][j] == 'S')
				map->s++;
			else if (map->line[k][j] == 'W')
				map->w++;
			else if (map->line[k][j] == 'E')
				map->e++;
			else if (map->line[k][j] == 'N')
				map->n++;
			else if (map->line[k][j] != '1' && map->line[k][j] != '0'
				&& map->line[k][j] != ' ' && map->line[k][j] != '\t')
			{
				printf("Error\n Unknown character \n");
				freee(map);
				exit(0);
			}
			j--;
		}
		k++;
	}
	ft_error(map);
}

void	get_map2(t_map *map, int *i)
{
	int	j;
	int	k;

	j = *i;
	k = 0;
	if (map->countlines_map == 0)
		file_err(3);
	map->map_copy = malloc(sizeof(char *) * (map->countlines_map + 1));
	if (!map->map_copy)
		exit(0);
	while (map->line[j])
	{
		map->map_copy[k] = map->line[j];
		j++;
		k++;
	}
	map->map_copy[k] = NULL;
}

void	verify_space(t_map *map, int i, int j)
{
	if ((map->map_copy[i + 1][j] == ' ' || map->map_copy[i + 1][j] == '\t'
			|| map->map_copy[i + 1][j] == '\n') || (map->map_copy[i
			- 1][j] == ' ' || map->map_copy[i - 1][j] == '\t' || map->map_copy[i
			- 1][j] == '\n') || (map->map_copy[i][j + 1] == ' '
			|| map->map_copy[i][j + 1] == '\t' || map->map_copy[i][j
			+ 1] == '\n') || (map->map_copy[i][j - 1] == ' '
			|| map->map_copy[i][j - 1] == '\t' || map->map_copy[i][j
			- 1] == '\n'))
	{
		printf("Error\n '0' is surrounded by spaces at line %d, column %d\n", i,
			j);
		freee(map);
		exit(0);
	}
}

void	check_spaces(t_map *map)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (i < map->countlines_map)
	{
		j = 0;
		len = ft_strlen(map->map_copy[i]) - 1;
		while (j < len)
		{
			if (map->map_copy[i][j] == '0')
			{
				verify_space(map, i, j);
			}
			j++;
		}
		i++;
	}
}

void	check_valid_map(t_map *map, int *i)
{
	check_first_last(map, i);
	check_map_borders(map, i);
	check_characters(map, i);
	get_map2(map, i);
	parse_map(map, *i);
	check_spaces(map);
}

void	check_map(t_map *map)
{
	int	i;

	i = 0;
	check_textures(map, i);
	check_fc(map, i);
	check_multiple(map);
	if (ft_find_map(map, &i) != 1)
	{
		check_valid_map(map, &i);
	}
	else
	{
		printf("Map doesn't exist!\n");
		freee(map);
		exit(0);
	}
}

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	*s;
	size_t			i;

	i = 0;
	s = (unsigned char *)str;
	while (i < n)
	{
		s[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

void	init_data(t_map *map, t_data *data)
{
	map->countlines = 0;
	map->line = NULL;
	map->count = malloc(sizeof(t_count));
	if (!map->count)
		return ;
	map->map_i = 0;
	map->map_j = 0;
	map->map = NULL;
	map->map_copy = NULL;
	map->ea_texture = NULL;
	map->we_texture = NULL;
	map->no_texture = NULL;
	map->so_texture = NULL;
	ft_memset(map->c_color, -1, sizeof(map->c_color));
	ft_memset(map->f_color, -1, sizeof(map->f_color));
	map->countlines_map = 0;
	map->file_name = NULL;
	map->s = 0;
	map->w = 0;
	map->e = 0;
	map->n = 0;
	map->x_p = 0;
	map->y_p = 0;
	data->map = map;
	map->data = data;
}

void	create_image(t_data *data)
{
	t_mlx	*img;

	img = malloc(sizeof(t_mlx));
	img->img = mlx_new_image(data->mlx, screenWidth, screenHeight);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	data->img = img;
}

int	povright(t_data *data) // used rotation matrices
{
	float oldDirX;
	float oldPlaneX;

	oldDirX = data->dirx;
	oldPlaneX = data->planeX;
	data->dirx = data->dirx * cos(0.1) - data->diry * sin(0.1);
	data->diry = oldDirX * sin(0.1) + data->diry * cos(0.1);
	data->planeX = data->planeX * cos(0.1) - data->planeY * sin(0.1);
	data->planeY = oldPlaneX * sin(0.1) + data->planeY * cos(0.1);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

int	povleft(t_data *data)
{
	float	oldDirX;
	float	oldPlaneX;

	oldDirX = data->dirx;
	oldPlaneX = data->planeX;
	data->dirx = data->dirx * cos(-0.1) - data->diry * sin(-0.1);
	data->diry = oldDirX * sin(-0.1) + data->diry * cos(-0.1);
	data->planeX = data->planeX * cos(-0.1) - data->planeY * sin(-0.1);
	data->planeY = oldPlaneX * sin(-0.1) + data->planeY * cos(-0.1);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

// void move_mini_player_front(t_data *data)
// {
//     // if (data->compass == 'N')
//     // {
//     //     if (data->map->map[data->player_i - 1][data->player_j] == '1')
//     //         return ;
//     //     data->map->map[data->player_i - 1][data->player_j] = 'N';
//     //     data->map->map[data->player_i][data->player_j] = '0';
//     //     data->player_i--;
//     // }
// }

int	left(t_data *data)
{
	float	newposx;
	float	newposy;

	// printf("before moved left dir x %f, dir y %f, posx %f, posy %f\n",
		// data->dirx, data->diry, data->posx, data->posy);
	newposx = data->posx + data->planeX * 0.1;
	newposy = data->posy + data->planeY * 0.1;
	// printf("after moved left dir x %f, dir y %f, posx %f, posy %f\n",
		// data->dirx, data->diry, newposx, newposy);
	if (data->map->map[(int)newposy][(int)data->posx] != '1')
		data->posy = newposy;
	if (data->map->map[(int)data->posy][(int)newposx] != '1')
		data->posx = newposx;
	// data->posx = newposx;
	// data->posy = newposy;
	data->minimap->mini_posx = (int)data->posx;
	data->minimap->mini_posy = (int)data->posy;
	// move_mini_player_front(data);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

int	right(t_data *data)
{
	float	newposx;
	float	newposy;

	// printf("before moved righ dir x %f, dir y %f, posx %f, posy %f\n",
		// data->dirx, data->diry, data->posx, data->posy);
	newposx = data->posx - data->planeX * 0.1;
	newposy = data->posy - data->planeY * 0.1;
	// printf("after moved right dir x %f, dir y %f, posx %f, posy %f\n",
		// data->dirx, data->diry, newposx, newposy);
	if (data->map->map[(int)newposy][(int)data->posx] != '1')
		data->posy = newposy;
	if (data->map->map[(int)data->posy][(int)newposx] != '1')
		data->posx = newposx;
	data->minimap->mini_posx = (int)data->posx;
	data->minimap->mini_posy = (int)data->posy;
	// move_mini_player_front(data);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

int	front(t_data *data)
{
	float	newposx;
	float	newposy;

	newposx = data->posx + data->dirx * 0.1;
	newposy = data->posy + data->diry * 0.1;
	if (data->map->map[(int)newposy][(int)data->posx] != '1')
		data->posy = newposy;
	if (data->map->map[(int)data->posy][(int)newposx] != '1')
		data->posx = newposx;
	data->minimap->mini_posx = (int)data->posx;
	data->minimap->mini_posy = (int)data->posy;
	// move_mini_player_front(data);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

int	back(t_data *data)
{
	float	newposx;
	float	newposy;

	newposx = data->posx - data->dirx * 0.1;
	newposy = data->posy - data->diry * 0.1;
	if (data->map->map[(int)newposy][(int)data->posx] != '1')
		data->posy = newposy;
	if (data->map->map[(int)data->posy][(int)newposx] != '1')
		data->posx = newposx;
	data->minimap->mini_posx = (int)data->posx;
	data->minimap->mini_posy = (int)data->posy;
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	return (0);
}

// int ray2(int keycode ,t_data *data)
// {
//     // raycast(data);
//     // (int)data;
//     // printf("key is %d\n", keycode);
//     // mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0,
	// 0);
//     // mlx_clear_window(data->mlx, data->mlx_win);
// 	// if (keycode == 97)
// 	// 	if(right(data) == 1)
//     //         return (0);
// 	// if (keycode == 100)
// 	// 	if(left(data) == 1)
//     //         return (0);
//     // if (keycode == 119)
// 	// 	if(front(data) == 1)
//     //         return (0);
// 	// if (keycode == 115)
// 	// 	if (back(data) == 1)
//     //         return (0);
// 	if (keycode == 65361)
// 		if (povleft(data) == 1)
//             return 0;
// 	if (keycode == 65363)
// 		if (povright(data) == 1)
//             return 0;
// 	// if (keycode == 65307)
// 	// 	exit(1);
// 	return 0;
// }

int	ray(int keycode, t_data *data)
{
	// raycast(data);
	// (int)data;
	// printf("key is %d\n", keycode);
	// mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	// mlx_clear_window(data->mlx, data->mlx_win);
	if (keycode == 97)
		if (right(data) == 1)
			return 0;
	if (keycode == 100)
		if (left(data) == 1)
			return 0;
	if (keycode == 119)
		if (front(data) == 1)
			return 0;
	if (keycode == 115)
		if (back(data) == 1)
			return 0;
	if (keycode == 65361)
		if (povleft(data) == 1)
			return 0;
	if (keycode == 65363)
		if (povright(data) == 1)
			return 0;
	if (keycode == 65307)
		exit(0);
	return 0;
}

int	delete_window(t_data *data)
{
	mlx_destroy_window(data->mlx, data->mlx_win);
	exit(0);
}

void	create_window(t_data *data, t_map *map, t_wall *wall)
{
	// void	*mlx;
	// void	*mlx_win;
	// t_map   map;
	// printf("map --> %s\n", map->map[0]);
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, screenWidth, screenHeight,
			"Hello world!");
	// draw_a_line(&img);
	ray_data_init(data, map, wall);
	player_init(data);
	data_init(data);
	mini_map(data);
	raycast(data);
	mlx_clear_window(data->mlx, data->mlx_win);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->img->img, 0, 0);
	mlx_hook(data->mlx_win, 2, 1L << 0, &ray, data);
	// mlx_hook(data->mlx_win, 2, 1L<<0, &ray2, data);
	mlx_hook(data->mlx_win, 17, 0, (void *)delete_window, data);
	mlx_loop(data->mlx);
}

int	main(int ac, char **av)
{
	t_map	*map;
	t_wall	wall;
	t_data	*data;

	if (ac <= 1)
		exit(0);
	if (ac != 2)
	{
		printf("Error\nnumber of args\n");
		exit(0);
	}
	map = malloc(sizeof(t_map));
	data = malloc(sizeof(t_data));
	check_file(av[1]);
	init_data(map, data);
	get_map(av[1], map);
	check_map(map);
	create_window(data, map, &wall);
	free_all(map);
	return (0);
}
