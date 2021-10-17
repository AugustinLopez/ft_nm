#include "ft_main.h"
#include "ft_mylib.h"
#include "ft_fatelf.h"
#include <stddef.h>
#include <unistd.h>

static int fatloop(void *ptr, t_fat *record, t_nmhandle *printer)
{
	char *address = ptr + record->address;
	int endian;
	int ret;

	printer->flag |= FLAG_FATELF;
	printer->architecture = record->architecture;
	printer->class = elf_identifier(address, record->size, &endian);
	if (printer->class == 64)
		ret = elf64_main(address, record->size, endian, printer);
	else if (printer->class == 32)
		ret = elf32_main(address, record->size, endian, printer);
	else {
		write(STDERR_FILENO, "unsupported FAT architecture\n", 30);
		ret = -1;
	}
	printer->flag &= ~FLAG_FATELF;
	printer->architecture = 0;
	return (ret);
}

int fatelf_main(void *ptr, size_t len, t_nmhandle *printer)
{
	uint8_t *head = ptr;
	size_t offset = 8;
	t_fat record[255];
	int nbr;

	nbr = head[6];
	for (int i = 0; i < nbr; i++) {
		if (offset + 24 > len) {
			write(STDERR_FILENO, "file is too short\n", 18);
			return (-1);
		}
		ft_memset(&(record[i]), 0, sizeof(t_fat));
		record[i].architecture = (head[offset + 1] << 8) + head[offset];
		for (int j = 0; j < 8; j++)
			record[i].address += (head[offset + 8 + j] << (j * 8)); 
		for (int j = 0; j < 8; j++)
			record[i].size += (head[offset + 16 + j] << (j * 8));
		if (record[i].address + record[i].size > len) {
			write(STDERR_FILENO, "file is too short\n", 18);
			return (-1);
		}
		offset += 24;
	}
    if (printer->flag & FLAG_MULTIPLE_ARG) {
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, printer->filename, ft_strlen(printer->filename));
		write(STDOUT_FILENO, ":\n", 2);
	}
	for (int i = 0; i < nbr; i++) {
		if (fatloop(ptr, &record[i], printer) == -1)
			return (-1);
	}
	return (0);
}