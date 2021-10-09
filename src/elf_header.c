/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 16:09:13 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/07 18:13:00 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <elf.h>
#include <unistd.h>

int elf_class(const void *ptr, size_t len)
{
	const Elf64_Ehdr *head64 = ptr;
	const Elf32_Ehdr *head32 = ptr;

	(void)head32;
	for (int i = 0; i < SELFMAG; i++) {
		if (head64->e_ident[i] != ELFMAG[i]) {
			write(STDERR_FILENO, "bad magic byte\n", 15);
			return (-1);
		}
	}
	if (head64->e_ident[EI_CLASS] == ELFCLASS64) {
		if ((head64->e_shoff > len) || (len - head64->e_shoff < head64->e_shnum * sizeof(Elf64_Shdr)))
			write(STDERR_FILENO, "bad full section offset\n", 24);
		else if ((head64->e_phoff > len) || (len - head64->e_phoff < head64->e_phnum * sizeof(Elf64_Phdr)))
			write(STDERR_FILENO, "bad full program offset\n", 24);
		else
			return (64);
		return (-1);
	}
	else if(head64->e_ident[EI_CLASS] == ELFCLASS32) {
		return (32);
	}
	write(STDERR_FILENO, "unsupported architecture\n", 25);
	return (-1);
}

char elf64_SymType(Elf64_Sym *sym, Elf64_Shdr *section) {
	
	uint64_t info = ELF64_ST_BIND(sym->st_info);
	uint64_t type = ELF64_ST_TYPE(sym->st_info);
	Elf64_Half index = sym->st_shndx;
	Elf64_Xword sflag;
	Elf64_Word stype;
	char c = '?';

	if (info == STB_GNU_UNIQUE)
		c = 'u';
	else if (info == STB_WEAK && type == STT_OBJECT)
		c = (index == SHN_UNDEF) ? 'v': 'V';
	else if (info == STB_WEAK)
		c = (index == SHN_UNDEF) ? 'w': 'W';
	else if (index == SHN_ABS)
		c = (info == STB_LOCAL) ? 'a': 'A';
	else if (index == SHN_COMMON)
		c = 'C';
	else if (index == SHN_UNDEF)
		c = 'U';
	if (c != '?')
		return (c);
	stype = section[index].sh_type;
	sflag = section[index].sh_flags;

	//refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/specialsections.html
	if (stype == SHT_NOBITS && sflag == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (sflag == 0)
		c = 'N';
	else if ((stype == SHT_PREINIT_ARRAY || stype == SHT_PROGBITS || stype == SHT_DYNAMIC)
	&& sflag == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (stype == SHT_PROGBITS && sflag == SHF_ALLOC)
		c = 'R';
	else if ((stype == SHT_PROGBITS || stype == SHT_INIT_ARRAY || stype == SHT_FINI_ARRAY)
	&& sflag == (SHF_ALLOC | SHF_WRITE))
		c = 'T';
	else if (stype == SHT_PROGBITS || sflag == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	if (ft_strchr("uvw?", c) == NULL && info == STB_LOCAL)
		c += 'a' - 'A';
	return (c);
	
}

int elf64_read(const void *ptr, size_t len)
{
	const Elf64_Ehdr *head = ptr;
	Elf64_Shdr *section;
	//char *section_tab;
	Elf64_Sym *sym;
	char *sym_tab;
	char c;

	section = (Elf64_Shdr *)((char *)head + head->e_shoff);
	if (head->e_shstrndx == SHN_UNDEF || (section[head->e_shstrndx].sh_offset > len))
		write(STDERR_FILENO, "bad index to the section string table\n", 37);
	//section_tab = (char *)head + section[head->e_shstrndx].sh_offset;
	for (unsigned int i = 0; i < head->e_shnum; i ++) {
		if (section[i].sh_type == SHT_SYMTAB) {
			if (section[i].sh_offset > len
			|| section[i].sh_link > head->e_shnum
			|| section[section[i].sh_link].sh_offset > len) {
				write(STDERR_FILENO, "bad section offset\n", 19);
				return (-1);
			}
			sym = (Elf64_Sym *)((char *)head + section[i].sh_offset);
			sym_tab = (char *)head + section[section[i].sh_link].sh_offset;
			for (unsigned int j = 0; j < section[i].sh_size / section[i].sh_entsize; j++) {
				c = elf64_SymType(&sym[j], section);
				if (ELF64_ST_TYPE(sym[j].st_info) != STT_FILE && c != '?' && sym[j].st_name != 0)
				{
					if (sym[j].st_value != 0)
						printf("%016lx %c %s\n", sym[j].st_value, c, &sym_tab[sym[j].st_name]);
					else if (ft_strchr("uUvVwW", c) == NULL)
						printf("0000000000000000 %c %s\n", c, &sym_tab[sym[j].st_name]);
					else
						printf("                 %c %s\n", c, &sym_tab[sym[j].st_name]);
				}
			//	else if (sym[j].st_name != 0)
			//		printf("%c %s\n",c, &sym_tab[sym[j].st_name] );
			}
		}
	}
	return (0);
}
