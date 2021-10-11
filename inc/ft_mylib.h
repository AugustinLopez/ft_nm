#ifndef FT_MYLIB_H
# define FT_MYLIB_H

# include <stdint.h>
# include <stddef.h>

# ifndef __42__
	# include <string.h>
	# define ft_memset memset
	# define ft_memcmp memcmp
    # define ft_strcmp strcmp
	# define ft_strchr strchr
	# define ft_strlen strlen
	# define ft_strncmp strncmp
# else
	void *ft_memset(void *b, int c, size_t len);
	int memcmp(const void *s1, const void *s2, size_t n);
# endif

#include <stdint.h>
#include <stddef.h>

typedef struct s_nmlist {
    struct s_nmlist *next;
    uint64_t index;
    uint64_t addr;
    char letter;
    const char *name;
} t_nmlist;

typedef struct s_nmhandle {
    t_nmlist *begin;
    t_nmlist *current;
    t_nmlist *end;
    size_t current_count;
    size_t total_count;
    uint64_t flag;
    int alive;
} t_nmhandle;

int handle_constructor(t_nmhandle *handler);
void handle_reset(t_nmhandle *handler);
void handle_destructor(t_nmhandle *handler);
int handle_add_fof(t_nmhandle *handler, uint64_t addr, char letter, const char *name);
void handle_print(t_nmhandle *handler, char *filename);

uint64_t my_endian(const void *number, size_t size, int endian);
# endif