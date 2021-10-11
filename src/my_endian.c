#include <stdint.h>
#include <stddef.h>
#include <elf.h>

uint64_t my_endian(const void *number, size_t size, int endian)
{
	uint64_t tmp = 0;
	const uint8_t *ptr = number;

	if (endian == ELFDATA2LSB)
		for (size_t i = size; i > 0; i--)
			tmp = ptr[i - 1] + tmp * 0x100;
	else
		for (size_t i = 0; i < size; i++)
			tmp = ptr[i] + tmp * 0x100;
	return (tmp);
}