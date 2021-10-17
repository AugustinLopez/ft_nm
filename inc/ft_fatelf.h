#ifndef FT_FATELF_H
# define FT_FATELF_H

# include <stdint.h>

# define FATELFMAG "\xFA\x70\x0E\x1F"
# define SFATELFMAG 4

typedef struct s_fat {
	uint16_t architecture;
	uint8_t osabi;
	uint8_t version;
	uint8_t wordsize;
	uint8_t byte_order;
	uint16_t alignment;
	uint64_t address;
	uint64_t size;
} t_fat;

#endif