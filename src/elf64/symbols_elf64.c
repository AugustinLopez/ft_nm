#include "ft_mylib.h"
#include "ft_elf64.h"
#include <unistd.h>

static unsigned char elf64_letter_from_section_name(const char *name)
{
	unsigned char c = '?';

	for (int i = 0; i < STT_COUNT; i++) {
		if (!ft_strncmp(stt[i].name, name, ft_strlen(stt[i].name))) {
			c = stt[i].letter;
			break ;
		}
	}
	return (c);
}

static unsigned char elf64_letter_from_section_info(const Elf64_Shdr *section)
{
	unsigned char c = '?';

	if (section->sh_type == SHT_NOBITS)
		c = (section->sh_flags & SHF_IA_64_SHORT) ? 'S' : 'B';
	else if (section->sh_type == SHT_IA_64_UNWIND)
		c = 'p';
	else if ((section->sh_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == SHF_ALLOC)
		c = 'R';
	else if ((section->sh_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == (SHF_ALLOC | SHF_WRITE))
		c = (section->sh_flags & SHF_IA_64_SHORT) ? 'G' : 'D';
	else if (section->sh_flags == 0)
		c = 'N';
	if ((section->sh_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	return (c);
}

static unsigned char elf64_letter_from_symbol(const Elf64_Sym *sym)
{
	unsigned char c = '?';

	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_TYPE(sym->st_info) == STT_GNU_IFUNC)
		c = 'i';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
			c = (sym->st_shndx == SHN_UNDEF) ? 'v' : 'V';
		else
			c = (sym->st_shndx == SHN_UNDEF) ? 'w' : 'W';
	}
	else if (sym->st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym->st_shndx == SHN_ABS)
		c = 'A';
	else if (sym->st_shndx == SHN_COMMON)
		c = 'C';
	return (c);
}


int elf64_symbols(t_m64 *elf, const Elf64_Shdr *section, const Elf64_Sym *symtab, const char *symname, t_nmhandle *printer)
{
	unsigned char c;

	for (unsigned int i = 1; i < section->sh_size / section->sh_entsize; i++) {
		if (ELF64_ST_TYPE(symtab[i].st_info) == STT_FILE
		|| ELF64_ST_TYPE(symtab[i].st_info) == STT_SECTION)
			continue ;
		c = elf64_letter_from_symbol(&symtab[i]);
		if (c == '?' && symtab[i].st_shndx >= elf->e_shnum) {
			write(STDERR_FILENO, "Unexpected index\n", 17);
			return (-1);
		}
		c = (c != '?') ? c : elf64_letter_from_section_name(elf->string_ptr + elf->section[symtab[i].st_shndx].sh_name);
		c = (c != '?') ? c : elf64_letter_from_section_info(&(elf->section[symtab[i].st_shndx])); 
		if (c == '?')
			continue ;
		if (ft_strchr("?uvwpi", c) == NULL && ELF64_ST_BIND(symtab[i].st_info) == STB_LOCAL)
			c += 'a' - 'A';
		if (handle_add_fof(printer, (uint64_t)symtab[i].st_value, c, symname + symtab[i].st_name) == -1)
			return (-1);
	}
	return (0);
}
