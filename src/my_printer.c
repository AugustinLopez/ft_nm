#include "ft_mylib.h"
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

static int ft_address(t_nmlist *left, t_nmlist *right)
{
	int ret;

    if (ft_strchr("Uwv", left->letter) != NULL && ft_strchr("Uwv", right->letter) == NULL)
        return (-1);
    else if (ft_strchr("Uwv", right->letter) != NULL && ft_strchr("Uwv", left->letter) == NULL)
        return (1);
    else if (left->addr > right->addr)
        return (1);
    else if (left->addr < right->addr) {
        return (-1);
	}
	ret = ft_strcmp(left->name, right->name);
    if (ret != 0)
        return (ret);
	else
	 	return (left->letter >= right->letter);
}

static int ft_name(t_nmlist *left, t_nmlist *right)
{
    int ret;

    ret = ft_strcmp(left->name, right->name);
    if (ret != 0)
        return (ret);
    if (ft_strchr("Uwv", left->letter) != NULL && ft_strchr("Uwv", right->letter) == NULL)
        return (-1);
    else if (ft_strchr("Uwv", right->letter) != NULL && ft_strchr("Uwv", left->letter) == NULL)
        return (1);
    else if (left->addr > right->addr)
        return (1);
    else if (left->addr < right->addr)
        return (-1);
	else
 		return (left->letter >= right->letter);
}

static int ft_raddress(t_nmlist *left, t_nmlist *right)
{
    return (ft_address(right, left));
}

static int ft_rname(t_nmlist *left, t_nmlist *right)
{
    return (ft_name(right, left));
}

void write_number(uint64_t number, char format)
{
    uint8_t buff[30];
    size_t i = 30;
    size_t j = (format == 'X') ? 16 : 8;

    if (format == 'd') {
        if (number == 0) {
           write(STDOUT_FILENO, "0", 1);
           return ;
        }
        while (number) {
            buff[--i] = number % 10 + '0';
            number /= 10;
        }
        write(STDOUT_FILENO, buff + i, 30 - i);
        return ;
    }
    ft_memset(buff, '0', 30);
    if (number == 0) {
        write(STDOUT_FILENO, buff, j);
        return ;
    }
    while (number) {
        buff[--i] = number % 16;
        if (buff[i] < 10)
            buff[i] += '0';
        else
            buff[i] += 'a' - 10;
        number /= 16;
    }
    write(STDOUT_FILENO, buff + 30 - j, j);
    return ;
}

void initial_print(t_nmhandle *handler)
{
    if (handler->flag & FLAG_FILENAME)
        return ;
    if (handler->flag & FLAG_FATELF) {
        write(STDOUT_FILENO, "\nMachine ", 9);
        write_number((uint64_t)handler->architecture, 'd');
        write(STDOUT_FILENO, ":\n", 2);
    }
    else if (handler->flag & FLAG_ARCH) {
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, handler->objectname, ft_strlen(handler->objectname));
        write(STDOUT_FILENO, ":\n", 2);
    }
    else if (handler->flag & FLAG_MULTIPLE_ARG) {
        write(STDOUT_FILENO, "\n", 1);
        write(STDOUT_FILENO, handler->filename, ft_strlen(handler->filename));
        write(STDOUT_FILENO, ":\n", 2);
    }
}

void secondary_print(t_nmhandle *handler, t_nmlist *elem) {
    char buff[4] = " x ";

    if ((handler->flag & FLAG_UNDEFINED) && ft_strchr("Uwv", elem->letter) == NULL)
        return ;
    else if ((handler->flag & FLAG_EXTERNAL) && ft_strchr("UwvABCDGINPRSTVW", elem->letter) == NULL)
        return ;
    if (handler->flag & FLAG_FILENAME) {
        write(STDOUT_FILENO, handler->filename, ft_strlen(handler->filename));
        write(STDOUT_FILENO, ":" , 1);
        if (handler->flag & FLAG_ARCH) {
            write(STDOUT_FILENO, handler->objectname, ft_strlen(handler->objectname));
            write(STDOUT_FILENO, ":" , 1);
        }
        else if (handler->flag & FLAG_FATELF) {
            write(STDOUT_FILENO, "Machine ", 9);
            write_number((uint64_t)handler->architecture, 'd');
            write(STDOUT_FILENO, ":" , 1);
        }
    }
    if (elem->addr != 0 && ft_strchr("Uwv", elem->letter) == NULL)
        write_number((uint64_t)elem->addr, (handler->class == 64) ? 'X' : 'x');
	else if (ft_strchr("Uwv", elem->letter) == NULL)
        write_number(0, (handler->class == 64) ? 'X' : 'x');
	else
        write(STDOUT_FILENO, "                ", (handler->class == 64) ? 16 : 8);
    buff[1] = elem->letter;
    write(STDOUT_FILENO, buff, 3);
    write(STDOUT_FILENO, elem->name, ft_strlen(elem->name));
    write(STDOUT_FILENO, "\n", 1);
}

static int mergesort(t_nmlist **tab, size_t len, int (*cmp)(t_nmlist *, t_nmlist *))
{
	t_nmlist **left;
	t_nmlist **right;
	size_t i;
	size_t j;

	if (len <= 1)
		return (0);
	left = malloc(len * sizeof(t_nmlist *));
	if (!left)
		return (-1);
	ft_memcpy(left, tab, len * sizeof(t_nmlist *));
	right = left + len / 2;
	if (mergesort(left, len / 2, cmp) == -1
	|| mergesort(right, len - len / 2, cmp) == -1) {
		free(left);
		return (-1);
	}
	i = 0;
	j = 0;
	while (i < len / 2 && j < len - len / 2) {
		if (cmp(left[i], right[j]) <= 0) {
			tab[i + j] = left[i];
			i++;
		}
		else {
			tab[i + j] = right[j];
			j++;
		}
	}
	for (;i < len / 2;i++)
		tab[i + j] = left[i];
	for (;j < len - len / 2;j++)
		tab[i + j] = right[j];
	free(left);
	return (0);
}



void handle_print(t_nmhandle *handler)
{
    t_nmlist *elem = handler->begin;
    t_nmlist **sorted;
	int ret;

    initial_print(handler);
	if (handler->current_count == 0) {
		write(STDERR_FILENO, "No symbols\n", 11);
		return ;
	}
    if ((sorted = malloc(handler->current_count * sizeof(t_nmlist *))) == NULL) {
        write(STDERR_FILENO, "Could not allocate memory to print\n", 35);
        return ;
    }
    for (size_t i = 0; i < handler->current_count; i++) {
        sorted[i] = elem;
        elem = elem->next;
    }
    if ((handler->flag & FLAG_NOSORT) == 0) {
        if (handler->flag & FLAG_NUMSORT)
            ret = mergesort(sorted, handler->current_count, (handler->flag & FLAG_RSORT) ? ft_raddress : ft_address);
        else
            ret = mergesort(sorted, handler->current_count, (handler->flag & FLAG_RSORT) ? ft_rname : ft_name);
    }
	if (ret == -1)
		write(STDERR_FILENO, "Could not allocate memory to sort\n", 34);
	else {
   		for (size_t i = 0; i < handler->current_count; i++)
    	    secondary_print(handler, sorted[i]);
	}
    free(sorted);
}
