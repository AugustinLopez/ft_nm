/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 08:55:01 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/04 16:07:02 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <unistd.h>

int main(int ac, char **av)
{
	size_t len;
	void *ptr;

	if (ac > 2) {
		write(STDERR_FILENO, "Error: unexpected number of arguments\n", 37);
		write(STDERR_FILENO, "usage: ./ft_nm [executable]\n", 28);
		return (-1);
	}
	if (load_argv(ac == 2 ? av[1] : "a.out", &ptr, &len) == -1)
		return (-1);
	unload_argv(ptr, len);
	return (0);
}
