#include <unistd.h>
#include <elf.h>
#include <stddef.h>

int elf_identifier(const void *ptr, size_t len, int *endian)
{
	const Elf32_Ehdr *head = ptr;
	int class;

	if (len < sizeof(*head)) {
		write(STDERR_FILENO, "file is too short\n", 18);
		return (-1);
	}
	for (int i = 0; i < SELFMAG; i++) {
		if (head->e_ident[i] != ELFMAG[i]) {
			write(STDERR_FILENO, "bad magic byte\n", 15);
			return (-1);
		}
	}
	if (head->e_ident[EI_DATA] != ELFDATA2LSB && head->e_ident[EI_DATA] != ELFDATA2MSB) {
			write(STDERR_FILENO, "unsupported endianness\n", 23);
			return (-1);
	}
	*endian = head->e_ident[EI_DATA];
	class = (int)head->e_ident[EI_CLASS];
	if (class != ELFCLASS64 && class != ELFCLASS32) {
		write(STDERR_FILENO, "unsupported architecture\n", 26);
		return (-1);
	}
	return (class);
}