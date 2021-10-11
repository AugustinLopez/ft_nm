#include "ft_main.h"
#include "ft_mylib.h"

int main(int ac, char **av)
{
	size_t len;
	void *ptr;
	int endian;
	int ret = 0;
	int mem = 0;
	t_nmhandle printer;

	if (handle_constructor(&printer) == -1)
		return (-1);
	if (ac == 1) {
		if ((ret = load_argv("a.out", &ptr, &len)) != -1) {
			ret = elf_identifier(ptr, len, &endian);
			if (ret != -1)
				ret = elf64_main(ptr, len, endian, &printer);
		}
		handle_print(&printer, NULL);
		unload_argv(ptr, len);
		mem = ret;
	}
	else {
		for (int i = 1; i < ac; i++) {
			if ((ret = load_argv(av[i], &ptr, &len)) != -1) {
				ret = elf_identifier(ptr, len, &endian);
				if (ret != -1)
					ret = elf64_main(ptr, len, endian, &printer);
			}
			if (mem == 0)
				mem += (ret == -1);
			if (printer.alive == 0)
				break ;
			handle_print(&printer, ac > 2 ? av[i] : NULL);
			unload_argv(ptr, len);
			handle_reset(&printer);
		}
	}
	if (printer.alive)
		handle_destructor(&printer);
	return (mem);
}
