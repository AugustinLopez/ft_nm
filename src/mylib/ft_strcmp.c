/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 10:07:22 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/03 20:49:48 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char *c1;
	unsigned char *c2;

	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	while (*c1 == *c2 && *c1 && *c2)
	{
		++c1;
		++c2;
	}
	return (*(unsigned char *)c1 - *(unsigned char *)c2);
}