/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 16:09:13 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/04 18:10:08 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.nm.h"
#include <elf.h>
#include <unistd.h>

int elf_class(const void *ptr, size_t len)
{
	const Elf64_Ehdr *head = ptr;
	
	for (int i = 0; i < SELFMAG; i++) {
		if (head->e_ident[i] != ELFMAG[i]) {
			write(STDERR_FILENO, "bad magic byte\n", 15);
			return (-1);
		}
	}
	if (head->e_ident[EI_CLASS] == ELFCLASS64)
		return (64);
	else if(head->e_ident[EI_CLASS] == ELFCLASS32)
		return (32);
	write(STDERR_FILENO, "unsupported architecture\n", 25);
	return (-1);
}

int read_elf64(const void *ptr, size_t len)
{
	const Elf64_Ehdr *head = ptr;
	Elf64_Shdr *section;
	Elf64_Syn *sym;

	if ((head->e_shoff > len) || (len - head->e_shoff < head->e_shnum * sizeof(Elf64_Shdr))) {
		write(STDERR_FILENO, "bad full section offset\n", 24);
		return (-1);
	}
	else if ((head->e_phoff > len) || (len - head->e_phoff < head->e_phnum * sizeof(Elf64_Phdr))) {
		write(STDERR_FILENO, "bad full program offset\n", 24);
		return (-1);
	}
	section = (char *)head + head->e_shoff;
	for (unsigned int i = 0; i < head->e_shnum; i ++) {
		if (section[i].sh_type == SHT_SYMTAB) {
			if ((char *)head + section[i].sh_offset > len) {
				write(STDER_FILENO, "bad section offset\n", 19);
				return (-1);
			}
			sym = (char *)head + section[i].sh_offset;
		}
	}
	
}
