/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meriem <meriem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 00:00:28 by meriem            #+#    #+#             */
/*   Updated: 2024/12/11 00:03:54 by meriem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void free_map(t_map *map)
{
    int i;

    i = 0;
    while (i < map->countlines)
    {
        free(map->line[i]);
        i++;
    }
    free(map->line);
}

void free_all(t_map *map)
{
    free_map(map);
    free(map->count);
}