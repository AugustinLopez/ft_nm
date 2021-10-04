/* ************************************************************************** */

/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:35:23 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/04 14:43:56 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
	#include "elf.h"
#else
	#include <linux/elf.h>
#endif
#include "woody.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int is_valid_elf64_header(const void *ptr, size_t len)
{
	const Elf64_Ehdr *header = ptr;
	int ret = -1;

	if (len < sizeof(*header))
		dprintf(STDERR_FILENO, "Error: file is too short\n");
	else if (ft_memcmp(header->e_ident, ELFMAG, SELFMAG) != 0)
		dprintf(STDERR_FILENO, "Error: invalid initial ELF identifier\n");
	else if (header->e_ident[EI_CLASS] != ELFCLASS64)
		dprintf(STDERR_FILENO, "Error: not a 64 bit file\n");
	else if (header->e_ident[EI_DATA] != ELFDATA2LSB && header->e_ident[EI_DATA] != ELFDATA2MSB)
		dprintf(STDERR_FILENO, "Error: invalid endianness\n");
	else if (header->e_type != ET_EXEC && header->e_type != ET_DYN)
		dprintf(STDERR_FILENO, "Error: not an executable file\n");
	else if (header->e_entry == 0)
		dprintf(STDERR_FILENO, "Error: no entry point\n");
	else if (len - header->e_shoff < header->e_shnum * sizeof(Elf64_Shdr))
		dprintf(STDERR_FILENO, "Error: too many section headers for file size\n");
	else if (len - header->e_phoff < header->e_phnum * sizeof(Elf64_Phdr))
		dprintf(STDERR_FILENO, "Error: too many program headers for file size\n");
	else
		ret = 0;
	return (ret);
}
