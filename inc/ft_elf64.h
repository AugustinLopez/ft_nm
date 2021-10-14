#ifndef FT_ELF64_H
# define FT_ELF64_H

#include "ft_mylib.h"
# include <stddef.h>
# include <elf.h>

typedef struct s_my64 {
	char *ptr;
	Elf64_Ehdr *head;
	Elf64_Shdr *section;
	char *string_ptr;
	uint64_t e_shoff;
	uint64_t e_shnum;
	size_t len;
	int endian;
} t_m64;

int elf64_head(t_m64 *elf);
int elf64_sections(t_m64 *elf, t_nmhandle *printer);
int elf64_symbols(t_m64 *elf, const Elf64_Shdr *section, const Elf64_Sym *symtab, const char *symname, t_nmhandle *printer);
#endif