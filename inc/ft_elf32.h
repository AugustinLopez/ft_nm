#ifndef FT_ELF32_H
# define FT_ELF32_H

#include "ft_mylib.h"
# include <stddef.h>
# include <elf.h>

typedef struct s_my32 {
	char *ptr;
	Elf32_Ehdr *head;
	Elf32_Shdr *section;
	char *string_ptr;
	uint64_t e_shoff;
	uint64_t e_shnum;
	size_t len;
	int endian;
} t_m32;

int elf32_head(t_m32 *elf);
int elf32_sections(t_m32 *elf, t_nmhandle *printer);
int elf32_symbols(t_m32 *elf, const Elf32_Shdr *section, const Elf32_Sym *symtab, const char *symname, t_nmhandle *printer);
#endif