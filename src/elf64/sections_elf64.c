#include "ft_mylib.h"
#include "ft_elf64.h"
#include <unistd.h>
#include <stddef.h>

int elf64_sections(t_m64 *elf, t_nmhandle *printer)
{
	const Elf64_Shdr *section = elf->section;
	uint64_t osym;
    uint64_t istr;
	uint64_t ostr;

	for (size_t i = 0; i < elf->e_shnum; i++) {
		if (section[i].sh_type != SHT_SYMTAB)
			continue ;
		if (section[i].sh_link > elf->e_shnum) {
			write(STDERR_FILENO, "bad section index\n", 19);
			return (-1);
		}
    	istr = section[i].sh_link;
		osym = my_endian(&(section[i].sh_offset), sizeof(section[i].sh_offset), elf->endian);
		ostr = my_endian(&(section[istr].sh_offset), sizeof(section[istr].sh_offset), elf->endian);
		if (osym > elf->len || ostr > elf->len) {
			write(STDERR_FILENO, "bad section offset\n", 20);
			return (-1);
		}
		elf64_symbols(elf, &section[i], (Elf64_Sym *)(elf->ptr + osym), (char *)(elf->ptr + ostr), printer);
	}
	return (0);
}