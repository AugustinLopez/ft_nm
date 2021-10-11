#include "ft_mylib.h"
#include "ft_elf64.h"

int elf64_main(void *ptr, size_t len, int endian, t_nmhandle *printer)
{
	t_m64 elf;

	ft_memset(&elf, 0, sizeof(elf));
	elf.endian = endian;
	elf.len = len;
	elf.ptr = ptr;
	elf.head = ptr;
	if(elf64_head(&elf) == -1
	|| elf64_sections(&elf, printer) == -1)
		return (-1);
	return (0);
}