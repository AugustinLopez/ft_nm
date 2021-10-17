#include <unistd.h>
#include <elf.h>
#include <stddef.h>
#include "ft_fatelf.h"
#include "ft_ar.h"

static inline int fatelf_identifier(const void *ptr, size_t len) 
{
	const uint8_t *head = ptr;

	if (len < 8)
		return (-1);
	for (int i = 0; i < SFATELFMAG; i++) {
		if (head[i] != (uint8_t)FATELFMAG[i])
			return (-1);
	}
	if (head[4] != 0 && head[5] != 1 && head[7] != 0)
		return (-1);
	return (0);
}

static inline int arch_identifier(const void *ptr, size_t len) 
{
	const uint8_t *head = ptr;

	if (len < 68)
		return (-1);
	for (int i = 0; i < SARCHMAG; i++) {
		if (head[i] != (uint8_t)ARCHMAG[i])
			return (-1);
	}
	if (head[SARCHMAG] != '/' && head[SARCHMAG  + 1] != ' ')
		return (-1);
	return (0);
}

int elf_identifier(const void *ptr, size_t len, int *endian)
{
	const Elf32_Ehdr *head = ptr;

	if (fatelf_identifier(ptr, len) == 0)
		return (1);
	else if (arch_identifier(ptr, len) == 0)
		return (2);
	if (len < sizeof(*head)) {
		write(STDERR_FILENO, "file is too short\n", 18);
		return (-1);
	}
	for (int i = 0; i < SELFMAG; i++) {
		if (head->e_ident[i] != (uint8_t)ELFMAG[i]) {
			write(STDERR_FILENO, "bad magic byte\n", 15);
			return (-1);
		}
	}
	if (head->e_ident[EI_DATA] != ELFDATA2LSB && head->e_ident[EI_DATA] != ELFDATA2MSB) {
			write(STDERR_FILENO, "unsupported endianness\n", 23);
			return (-1);
	}
	*endian = head->e_ident[EI_DATA];
	if (head->e_ident[EI_CLASS] == ELFCLASS64)
		return (64);
	else if (head->e_ident[EI_CLASS] == ELFCLASS32)
		return (32);
	write(STDERR_FILENO, "unsupported architecture\n", 26);
	return (-1);
}