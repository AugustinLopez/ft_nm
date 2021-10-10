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
#include <elf.h>
#include <unistd.h>

uint64_t my_endian(const void *number, size_t size, int endian)
{
	uint64_t tmp = 0;
	const uint8_t *ptr = number;

	if (endian == ELFDATA2LSB)
		for (size_t i = size; i > 0; i--)
			tmp = ptr[i - 1] + tmp * 0x100;
	else
		for (size_t i = 0; i < size; i++)
			tmp = ptr[i] + tmp * 0x100;
	return (tmp);
}

int elf_from_head(const void *ptr, size_t len, int *endian)
{
	const Elf32_Ehdr *head = ptr;
	int class;

	if (len < sizeof(*head)) {
		write(STDERR_FILENO, "file is too short\n", 18);
		return (-1);
	}
	for (int i = 0; i < SELFMAG; i++) {
		if (head->e_ident[i] != ELFMAG[i]) {
			write(STDERR_FILENO, "bad magic byte\n", 15);
			return (-1);
		}
	}
	if (head->e_ident[EI_DATA] != ELFDATA2LSB && head->e_ident[EI_DATA] != ELFDATA2MSB) {
			write(STDERR_FILENO, "unsupported endianness\n", 23);
			return (-1);
	}
	*endian = head->e_ident[EI_DATA];
	class = (int)head->e_ident[EI_CLASS];
	if (class != ELFCLASS64 && class != ELFCLASS32) {
		write(STDERR_FILENO, "unsupported architecture\n", 26);
		return (-1);
	}
	return (class);
}

int main(int ac, char **av)
{
	size_t len;
	void *ptr;
	int endian;
	int ret = 0;
	int mem = 0;

	//No argument provided: try reading a.out
	if (ac == 1) {
		if ((ret = load_argv("a.out", &ptr, &len)) != -1) {
			ret = elf_from_head(ptr, len, &endian);
			if (ret != -1)
				ret = elf64_main(ptr, len, endian);
				//printf("Elf%d with %s endian\n", 32 * ret, endian == ELFDATA2LSB ? "little":"big");
		}
		unload_argv(ptr, len);
		return (ret);
	}

	//argument provided: read all of them
	for (int i = 1; i < ac; i++) {
		if ((ret = load_argv(av[i], &ptr, &len)) != -1) {
			ret = elf_from_head(ptr, len, &endian);
			if (ret != -1)
				ret = elf64_main(ptr, len, endian);
				//printf("Elf%d with %s endian\n", 32 * ret, endian == ELFDATA2LSB ? "little":"big");
		}
		if (mem == 0)
			mem += (ret == -1);
		unload_argv(ptr, len);
	}
	return (mem);
}
