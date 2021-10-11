#ifndef FT_MAIN_H
# define FT_MAIN_H

# include <stddef.h>
# include "ft_mylib.h"

int load_argv(const char *filename, void **ptr, size_t *len);
int unload_argv(void *ptr, size_t len);
int elf_identifier(const void *ptr, size_t len, int *endian);
int elf64_main(void *ptr, size_t len, int endian, t_nmhandle *printer);

#endif
