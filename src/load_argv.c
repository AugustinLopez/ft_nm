/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <aulopez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 11:20:31 by aulopez           #+#    #+#             */
/*   Updated: 2021/10/04 16:05:34 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int load_argv(const char *filename, void **ptr, size_t *len)
{
	int fd;
	struct stat buf;
	int ret = -1;

	if ((fd = open(filename, O_RDONLY)) < 0)
		write(STDERR_FILENO, "could not open file\n", 20);
	else if (fstat(fd, &buf) < 0)
		write(STDERR_FILENO, "fstat failure\n", 14);
	else if (S_ISDIR(buf.st_mode))
		write(STDERR_FILENO, "file is a directory\n", 20);
	else if ((*len = buf.st_size) == 0)
		write(STDERR_FILENO, "file length is 0\n", 17);
	else if ((*ptr = mmap(0, *len, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		write(STDERR_FILENO, "mmap failure\n", 13);
	else
		ret = 0;

	if (fd >= 0 && close(fd) < 0)
		write(STDERR_FILENO, "close failure\n", 14);
	return (ret);
}

int unload_argv(void *ptr, size_t len)
{
	if (ptr != NULL && munmap(ptr, len) < 0) {
		write(STDERR_FILENO, "munmmap failure\n", 16);
		return (-1);
	}
	return (0);
}
