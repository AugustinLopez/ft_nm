#include "ft_mylib.h"
#include "ft_elf32.h"

int elf32_main(void *ptr, size_t len, int endian, t_nmhandle *printer)
{
	t_m32 elf;

	ft_memset(&elf, 0, sizeof(elf));
	elf.endian = endian;
	elf.len = len;
	elf.ptr = ptr;
	elf.head = ptr;
	if(elf32_head(&elf) == -1
	|| elf32_sections(&elf, printer) == -1)
		return (-1);
	if (printer->alive) {
		handle_print(printer);
		handle_reset(printer);
	}
	else
		return (-1);
	return (0);
}