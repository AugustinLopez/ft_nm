#include "ft_nm.h"
#include <elf.h>
#include <unistd.h>

#include <stdio.h>

static int elf64_head_check(t_m64 *elf) {
	const Elf64_Ehdr *head = elf->head;
	uint64_t offset;
	uint64_t num;

	if (elf->len < sizeof(*head)) {
		write(STDERR_FILENO, "file is too short\n", 18);
		return (-1);
	}
	elf->e_shoff = my_endian(&(head->e_shoff), sizeof(head->e_shoff), elf->endian);
	elf->e_shnum = my_endian(&(head->e_shnum), sizeof(head->e_shnum), elf->endian);
	if ((elf->e_shoff > elf->len) || (elf->len - elf->e_shoff < elf->e_shnum * sizeof(Elf64_Shdr))) {
		write(STDERR_FILENO, "bad sections offset\n", 20);
		return (-1);
	}
	offset = my_endian(&(head->e_phoff), sizeof(head->e_phoff), elf->endian);
	num = my_endian(&(head->e_phnum), sizeof(head->e_phnum), elf->endian);
	if ((offset > elf->len) || (elf->len - offset < num * sizeof(Elf64_Phdr))) {
		write(STDERR_FILENO, "bad program offset\n", 19);
		return (-1);
	}
	num = my_endian(&(head->e_shstrndx), sizeof(head->e_shstrndx), elf->endian);
	if (num > elf->e_shnum || num == SHN_UNDEF) {
		write(STDERR_FILENO, "bad string table section offset\n", 33);
		return (-1);
	}
	elf->section = (void *)(elf->ptr + elf->e_shoff);
	offset = my_endian(&(elf->section[num].sh_offset), sizeof(elf->section[num].sh_offset), elf->endian);
	if (offset > elf->len) {
		printf("%zu %zu\n", offset, elf->len);
		write(STDERR_FILENO, "bad string table section offset\n", 33);
		return (-1);
	}
	elf->string_ptr = elf->ptr + elf->section[num].sh_offset;
	return (0);
}

struct section_to_type {
	char *name;
	char letter;
};

static const struct section_to_type stt[19] =
{
  {".bss", 'B'},
  {"code", 'T'},		/* MRI .text */
  {".data", 'D'},
  {"*DEBUG*", 'N'},
  {".debug", 'N'},		/* MSVC's .debug (non-standard debug syms) */
  {".drectve", 'I'},		/* MSVC's .drective section */
  {".edata", 'E'},		/* MSVC's .edata (export) section */
  {".fini", 'T'},		/* ELF fini section */
  {".idata", 'I'},		/* MSVC's .idata (import) section */
  {".init", 'T'},		/* ELF init section */
  {".pdata", 'P'},		/* MSVC's .pdata (stack unwind) section */
  {".rdata", 'R'},		/* Read only data.  */
  {".rodata", 'R'},		/* Read only data.  */
  {".sbss", 'S'},		/* Small BSS (uninitialized data).  */
  {".scommon", 'C'},		/* Small common.  */
  {".sdata", 'G'},		/* Small initialized data.  */
  {".text", 'T'},
  {"vars", 'D'},		/* MRI .data */
  {"zerovars", 'B'},		/* MRI .bss */
};

char elf64_sym_hard(const Elf64_Shdr *section, const char *s_name)
{
	char c = '?';
	Elf64_Xword sflag;
	Elf64_Word stype;

	sflag = section->sh_flags;
	stype = section->sh_type;
	//refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/specialsections.html
	if (stype == SHT_NOBITS && sflag == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (sflag == 0)
		c = 'N';
	else if ((stype == SHT_PREINIT_ARRAY || stype == SHT_PROGBITS || stype == SHT_DYNAMIC
	|| stype == SHT_INIT_ARRAY || stype == SHT_FINI_ARRAY)
	&& sflag == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (stype == SHT_PROGBITS && sflag == SHF_ALLOC)
		c = 'R';
	else if ((stype == SHT_PROGBITS)
	&& (sflag == (SHF_ALLOC | SHF_WRITE) || sflag == (SHF_ALLOC | SHF_EXECINSTR)))
		c = 'T';
	else {
		for (int i = 0; i < 19; i++) {
			if (!ft_strncmp(stt[i].name, s_name, ft_strlen(stt[i].name))) {
				c = stt[i].letter;
				break ;
			}
		}
	}
	return (c);
}

char elf64_sym_easy(const Elf64_Sym *sym)
{
	char c = '?';

	if (ELF64_ST_BIND(sym->st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
			c = (sym->st_shndx == SHN_UNDEF) ? 'v' : 'V';
		else
			c = (sym->st_shndx == SHN_UNDEF) ? 'w': 'W';
	}
	else if (sym->st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym->st_shndx == SHN_ABS)
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'a': 'A';
	else if (sym->st_shndx == SHN_COMMON)
		c = (ELF64_ST_BIND(sym->st_info) == STB_LOCAL) ? 'c': 'C';
	return (c);
}


int elf64_sym_loop(t_m64 *elf, const Elf64_Shdr *elem, uint64_t offset_sym, uint64_t offset_link)
{
	const Elf64_Sym *sym = (void *)(elf->ptr + offset_sym);
	const char *s_sym = (void *)(elf->ptr + offset_link);
	const char *elem_name = elf->string_ptr + elem->sh_name;
	char c = 'a';

	for (unsigned int i = 0; i < elem->sh_size / elem->sh_entsize; i++) {
		/*if (sym[i].st_shndx > elf->len) {
			printf("%d\n", sym[i].st_shndx);
			write(STDERR_FILENO, "bad section index\n", 19);
			return (-1);
		}*/
		if (ELF64_ST_TYPE(sym[i].st_info) == STT_FILE || sym[i].st_name == 0)
			continue ;
		c = elf64_sym_easy(&sym[i]);
		c = c != '?' ? c : elf64_sym_hard(&(elf->section[sym[i].st_shndx]), elem_name);
		if (ft_strchr("uvw?", c) == NULL && ELF64_ST_BIND(sym[i].st_info) == STB_LOCAL)
			c += 'a' - 'A';
		if (c == '?')
			continue ;
		else if (sym[i].st_value != 0 && ft_strchr("Uw", c) == NULL)
			printf("%016lx %c %s\n", sym[i].st_value, c, s_sym + sym[i].st_name);
		else if (ft_strchr("Uw", c) == NULL)
			printf("0000000000000000 %c %s\n", c, s_sym + sym[i].st_name);
		else
			printf("                 %c %s\n", c, s_sym + sym[i].st_name);
	}
	return (0);
}

int elf64_section_loop(t_m64 *elf)
{
	const Elf64_Shdr *section = elf->section;
	uint64_t offset_sym;
	uint64_t offset_link;

	for (size_t i = 0; i < elf->e_shnum; i++) {
		if (section[i].sh_type != SHT_SYMTAB)
			continue ;
		if (section[i].sh_link > elf->e_shnum) {
			write(STDERR_FILENO, "bad section index\n", 19);
			return (-1);
		}
		offset_sym = my_endian(&(section[i].sh_offset), sizeof(section[i].sh_offset), elf->endian);
		offset_link = section[i].sh_link;
		offset_link = my_endian(&(section[offset_link].sh_offset), sizeof(section[offset_link].sh_offset), elf->endian);
		if (offset_link > elf->len || offset_sym > elf->len) {
			write(STDERR_FILENO, "bad section offset\n", 20);
			return (-1);
		}
		elf64_sym_loop(elf, &section[i], offset_sym, offset_link);
		//printf("%zu. %s\n", i, elf->string_ptr  + section[i].sh_name);
	}
	return (0);
}

int elf64_main(void *ptr, size_t len, int endian)
{
	t_m64 elf;

	ft_memset(&elf, 0, sizeof(elf));
	elf.endian = endian;
	elf.len = len;
	elf.ptr = ptr;
	elf.head = ptr;
	if(elf64_head_check(&elf) == -1
	|| elf64_section_loop(&elf) == -1)
		return (-1);
	return (0);
}