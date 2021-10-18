#include "ft_main.h"
#include "ft_mylib.h"

static int mainloop(t_nmhandle *printer)
{
	size_t len;
	void *ptr;
	int endian;
	int ret;

	if (load_argv(printer->filename, &ptr, &len) == -1)
		return (-1);
	printer->class = elf_identifier(ptr, len, &endian);
	if (printer->class == 1)
		ret = fatelf_main(ptr, len, printer);
	else if (printer->class == 2)
		ret = arch_main(ptr, len, printer);
	else if (printer->class == 64)
		ret = elf64_main(ptr, len, endian, printer);
	else if (printer->class == 32)
		ret = elf32_main(ptr, len, endian, printer);
	unload_argv(ptr, len);
	return (ret);
}

int main(int ac, char **av)
{
	int ret = 0;
	int mem = 0;
	int stopopt = 0;
	t_nmhandle printer;

	ret = handle_constructor(&printer, ac, av);
	if (ret <= 0)
		return (ret);
	if (ret == 1) {
		printer.filename = "a.out";
		mem = mainloop(&printer);
	}
	else {
		for (int i = 1; i < ac; i++) {
			if (stopopt == 0) {
				if (ft_strcmp("--", av[i]) == 0) {
					stopopt = 1;
					continue ;
				}
				else if (av[i][0] == '-')
					continue ;
			}
			printer.filename = av[i];
			ret = mainloop(&printer);
			mem = (mem == 0) ? -(ret == -1) : mem;
			if (printer.alive == 0)
				break ;
		}
	}
	if (printer.alive)
		handle_destructor(&printer);
	return (mem);
}
