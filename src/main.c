/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 08:55:01 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/04 21:34:26 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <unistd.h>

int main(int ac, char **av)
{
	size_t len;
	void *ptr;
	int ret;

	if (ac == 1) {
		if (load_argv("a.out", &ptr, &len) == -1)
			return (-1);
		return (unload_argv(ptr, len));
	}
	for (int i = 1; i < ac; i++) {
		if (load_argv(av[i], &ptr, &len) == -1)
			continue ;
		ret = elf_class(ptr, len);
		if (ret == 64)
			elf64_read(ptr, len);
		unload_argv(ptr, len);
	}
	return (0);
}
