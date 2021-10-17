#include "ft_main.h"
#include "ft_mylib.h"
#include <stddef.h>
#include <unistd.h>

static inline int filesize(uint8_t *ptr, int limit)
{
	int nbr = 0;

	for (int i = 0; i < limit; i++) {
		if (ptr[i] == ' ') {
			for (int j = i; j < limit; j++) {
				if (ptr[i] != ' ')
					return (-1);
			}
			break ;
		}
		else if (ft_strchr("0123456789", ptr[i]) == NULL)
			return (-1);
		nbr = ptr[i] - '0' + nbr * 10;
	}
	return (nbr);
}


static inline uint8_t *filename(uint8_t *tar, uint8_t *src, uint8_t *extended, char sextended)
{
	int nbr;
	int i;

	nbr = 16;
	if (src[0] == '/') {
		nbr = filesize(src + 1, 15);
		if (nbr > sextended)
			return (NULL);
		src = extended + nbr;
		nbr = 256;
	}
	for (i = 0; i < nbr; i++) {
		if (src[i] == '/') {
			tar[i] = '\0';
			break ;
		}
		tar[i] = src[i];
	}
	if (tar[i] != 0)
		return (NULL);
	return(tar);
}

static int arloop(uint8_t *ptr, size_t size, t_nmhandle *printer)
{
	char *address = (char *)ptr;
	int endian;
	int ret;

	printer->flag |= FLAG_ARCH;
	printer->class = elf_identifier(address, size, &endian);
	if (printer->class == 64)
		ret = elf64_main(address, size, endian, printer);
	else if (printer->class == 32)
		ret = elf32_main(address, size, endian, printer);
	else {
		write(STDERR_FILENO, "unsupported archive architecture\n", 33);
		ret = -1;
	}
	printer->flag &= ~FLAG_ARCH;
	return (ret);
}

int archloop(uint8_t *head, size_t len, size_t *off, uint8_t *extended, int sextended, t_nmhandle *printer)
{
	uint8_t buff[257];
	uint8_t *file;
	int size;
	int ret;

	if (*off + 60 > len) {
		write(STDERR_FILENO, "Unexpected offset\n", 18);
		return (-1);
	}
	ft_memset(buff, 0, sizeof(buff));
	file = filename((uint8_t *)buff, head + *off, extended, sextended);
	size = filesize(head + *off + 48, 10);
	if (size < 0 || *off + 60 + size > len) {
		write(STDERR_FILENO, "Unexpected offset\n", 18);
		return (-1);
	}
	printer->objectname = (char *)file;
	ret = arloop(head + *off + 60, (size_t)size, printer);
	printer->objectname = NULL;
	*off += 60 + (size_t)size;
	return(0);
}

int arch_main(void *ptr, size_t len, t_nmhandle *printer)
{
	uint8_t *head = ptr;
	int ret = filesize(head + 56, 10);
	size_t offset;
	uint8_t *extended = NULL;
	int sextended = 0;

	offset = 68 + (size_t)ret;
	if (ret < 0 || offset > len) {
		write(STDERR_FILENO, "Unexpected offset\n", 18);
		return (-1);
	}
	if (ft_memcmp(head + offset, "//", 2) == 0) {
		extended = head + offset + 60;
		sextended = filesize(head + offset + 48, 10);
		offset += 60 + (size_t)sextended;
		if (sextended < 0 || offset > len) {
			write(STDERR_FILENO, "Unexpected offset\n", 18);
			return (-1);
		}
	}
    if (printer->flag & FLAG_MULTIPLE_ARG) {
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, printer->filename, ft_strlen(printer->filename));
		write(STDOUT_FILENO, ":\n", 2);
	}
	while (offset < len) {
		ret = archloop(head, len, &offset, extended, sextended, printer);
		if (ret == -1)
			break ;
	}
	return (0);
}