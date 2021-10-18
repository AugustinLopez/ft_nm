#ifndef FT_MYLIB_H
# define FT_MYLIB_H

# include <stdint.h>
# include <stddef.h>

# define FT_NM_HELP_MSG \
    "usage: ./ft_nm [-hAonvrpug] [Elf file]\n\n" \
    "-h    \tPrint this help\n" \
    "-A/o  \tPrecede each symbol by the input file name\n" \
    "-n/v  \tSort by address value first, name second\n" \
    "-r    \tReverse sort\n" \
    "-p    \tDo not sort\n" \
    "-u    \tDisplay only undefined symbols\n" \
    "-g    \tDisplay only external symbols\n"

# define FLAG_MULTIPLE_ARG 0x1
# define FLAG_FATELF 0x2
# define FLAG_ARCH 0x4
# define FLAG_FILENAME 0x8
# define FLAG_NOSORT 0x10
# define FLAG_RSORT 0x20
# define FLAG_NUMSORT 0x40
//# define FLAG_ARMAP 0x80
# define FLAG_UNDEFINED 0x100
# define FLAG_EXTERNAL 0x200
# define __42__

# ifndef __42__
	# include <string.h>
	# define ft_memset memset
	# define ft_memcmp memcmp
    # define ft_memcpy memcpy
    # define ft_strcmp strcmp
	# define ft_strchr strchr
	# define ft_strlen strlen
	# define ft_strncmp strncmp
# else
	void    *ft_memset(void *b, int c, size_t len);
	int     ft_memcmp(const void *s1, const void *s2, size_t n);
    void    *ft_memcpy(void *s1, const void *s2, size_t n);
    char	*ft_strchr(const char *s, int c);
    int	    ft_strcmp(const char *s1, const char *s2);
    size_t	ft_strlen(const char *s);
    int     ft_strncmp(const char *s1, const char *s2, size_t n);
# endif

struct section_to_type {
	char *name;
	char letter;
};

# define STT_COUNT 21
static const struct section_to_type stt[STT_COUNT] =
{
  {".bss", 'B'},
  {"code", 'T'},		/* MRI .text */
  {".data", 'D'},
  {"*DEBUG*", 'N'},
  {".debug", 'N'},		/* MSVC's .debug (non-standard debug syms) */
  {".drectve", 'I'},		/* MSVC's .drective section */
  {".edata", 'E'},		/* MSVC's .edata (export) section */
  {".init_array", '?'},
  {".fini_array", '?'},
  //{".eh_frame", 'B'},
  {".fini", 'T'},		
  {".init", 'T'},	
  {".idata", 'I'},		/* MSVC's .idata (import) section */
  {".pdata", 'P'},		/* MSVC's .pdata (stack unwind) section */
  {".rdata", 'R'},		/* Read only data.  */
  {".rodata", 'R'},		/* Read only data.  */
  {".sbss", 'S'},		/* Small BSS (uninitialized data).  */
  {".scommon", 'C'},		/* Small common.  */
  {".sdata", 'G'},		/* Small initialized data.  */
  {".text", 'T'},
  {"vars", 'D'},		/* MRI .data */
  {"zerovars", 'B'},		/* MRI .bss */
};

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
    char *filename;
    size_t current_count;
    size_t total_count;
    uint64_t flag;
    uint16_t architecture;
    char *objectname;
    int class;
    int alive;
} t_nmhandle;

int handle_constructor(t_nmhandle *handler, int ac, char **av);
void handle_reset(t_nmhandle *handler);
void handle_destructor(t_nmhandle *handler);
int handle_add_fof(t_nmhandle *handler, uint64_t addr, char letter, const char *name);
void handle_print(t_nmhandle *handler);

uint64_t my_endian(const void *number, size_t size, int endian);
# endif