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

# ifndef __42__
	# include <string.h>
	# define ft_memset memset
	# define ft_memcmp memcmp
	# define ft_strchr strchr
# else
	void *ft_memset(void *b, int c, size_t len);
	int memcmp(const void *s1, const void *s2, size_t n);
# endif

int load_argv(const char *filename, void **ptr, size_t *len);
int unload_argv(void *ptr, size_t len);
#endif
