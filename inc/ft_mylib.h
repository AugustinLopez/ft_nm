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

struct section_to_type {
	char *name;
	char letter;
};

//https://sourceware.org/git/?p=binutils-gdb.git;a=blob_plain;f=bfd/syms.c;hb=a288c270991de1578ad28ac312120f4167347234

# define STT_COUNT 22
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
  {".eh_frame", 'B'},
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
    size_t current_count;
    size_t total_count;
    uint64_t flag;
    int class;
    int alive;
} t_nmhandle;

int handle_constructor(t_nmhandle *handler);
void handle_reset(t_nmhandle *handler);
void handle_destructor(t_nmhandle *handler);
int handle_add_fof(t_nmhandle *handler, uint64_t addr, char letter, const char *name);
void handle_print(t_nmhandle *handler, char *filename);

uint64_t my_endian(const void *number, size_t size, int endian);
# endif