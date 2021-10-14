#include "ft_elf32.h"
#include "ft_mylib.h"
#include <unistd.h>

int elf32_head(t_m32 *elf) {
	const Elf32_Ehdr *head = elf->head;
	uint32_t offset;
	uint32_t num;

	if (elf->len < sizeof(*head)) {
		write(STDERR_FILENO, "file is too short\n", 18);
		return (-1);
	}
	elf->e_shoff = my_endian(&(head->e_shoff), sizeof(head->e_shoff), elf->endian);
	elf->e_shnum = my_endian(&(head->e_shnum), sizeof(head->e_shnum), elf->endian);
	if ((elf->e_shoff > elf->len) || (elf->len - elf->e_shoff < elf->e_shnum * sizeof(Elf32_Shdr))) {
		write(STDERR_FILENO, "bad sections offset\n", 20);
		return (-1);
	}
	offset = my_endian(&(head->e_phoff), sizeof(head->e_phoff), elf->endian);
	num = my_endian(&(head->e_phnum), sizeof(head->e_phnum), elf->endian);
	if ((offset > elf->len) || (elf->len - offset < num * sizeof(Elf32_Phdr))) {
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
		write(STDERR_FILENO, "bad string table section offset\n", 33);
		return (-1);
	}
	elf->string_ptr = elf->ptr + elf->section[num].sh_offset;
	return (0);
}
