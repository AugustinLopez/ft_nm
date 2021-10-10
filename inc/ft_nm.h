/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:32:31 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/07 17:05:15 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stddef.h>
# include <elf.h>

# ifndef __42__
	# include <string.h>
	# define ft_memset memset
	# define ft_memcmp memcmp
	# define ft_strchr strchr
	# define ft_strlen strlen
	# define ft_strncmp strncmp
# else
	void *ft_memset(void *b, int c, size_t len);
	int memcmp(const void *s1, const void *s2, size_t n);
# endif

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

uint64_t my_endian(const void *number, size_t size, int endian);
int load_argv(const char *filename, void **ptr, size_t *len);
int unload_argv(void *ptr, size_t len);
int elf64_main(void *ptr, size_t len, int endian);
#endif
