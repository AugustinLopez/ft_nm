#ifndef FT_ELF64_H
# define FT_ELF64_H

#include "ft_mylib.h"
# include <stddef.h>
# include <elf.h>

struct section_to_type {
	char *name;
	char letter;
};

//https://sourceware.org/git/?p=binutils-gdb.git;a=blob_plain;f=bfd/syms.c;hb=a288c270991de1578ad28ac312120f4167347234

# define STT_COUNT 19
static const struct section_to_type stt[STT_COUNT] =
{
  {".bss", 'B'},
  {"code", 'T'},		/* MRI .text */
  {".data", 'D'},
  {"*DEBUG*", 'N'},
  {".debug", 'N'},		/* MSVC's .debug (non-standard debug syms) */
  {".drectve", 'I'},		/* MSVC's .drective section */
  {".edata", 'E'},		/* MSVC's .edata (export) section */
  {".fini", 'T'},		/* ELF fini section */
  {".idata", 'I'},		/* MSVC's .idata (import) section */
  {".init", 'T'},		/* ELF init section */
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

typedef struct s_my64 {
	char *ptr;
	Elf64_Ehdr *head;
	Elf64_Shdr *section;
	char *string_ptr;
	uint64_t e_shoff;
	uint64_t e_shnum;
	size_t len;
	int endian;
} t_m64;

int elf64_head(t_m64 *elf);
int elf64_sections(t_m64 *elf, t_nmhandle *printer);
int elf64_symbols(t_m64 *elf, const Elf64_Shdr *section, const Elf64_Sym *symtab, const char *symname, t_nmhandle *printer);
#endif