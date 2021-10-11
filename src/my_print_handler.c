#include "ft_mylib.h"
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

static t_nmlist *handle_add(t_nmhandle *handler, uint64_t addr, char letter, const char *name)
{
    t_nmlist *elem = NULL;

    if (handler == NULL)
        return (NULL);
    if (handler->current == NULL) {
        if (handler->begin == NULL) {
            if ((elem = malloc(sizeof(*elem))) == NULL)
                return (NULL);
            ft_memset(elem, 0, sizeof(*elem));
            handler->total_count = 1;
            handler->begin = elem;
            handler->end = elem;
        }
        else {
            elem = handler->begin;
        }
        handler->current = elem;
        handler->current_count = 1;
    }
    else if (handler->current->next == NULL) {
        if ((elem = malloc(sizeof(*elem))) == NULL)
            return (NULL);
        ft_memset(elem, 0, sizeof(*elem));
        handler->total_count += 1;
        handler->current_count = handler->total_count;
        handler->end = elem;
        handler->current->next = elem;
        handler->current = elem;
    }
    else {
        handler->current_count += 1;
        handler->current = handler->current->next;
    }
    handler->current->addr = addr;
    handler->current->letter = letter;
    handler->current->name = name;
    return (handler->current);
}

void handle_destructor(t_nmhandle *handler)
{
    t_nmlist *elem = handler->begin;
    t_nmlist *next;

    while (elem) {
        next = elem->next;
        free(elem);
        elem = next;
    }
    ft_memset(handler, 0, sizeof(*handler));
    handler->alive = 0; //unecessary but kept for easier reading
}

//free on fail
int handle_add_fof(t_nmhandle *handler, uint64_t addr, char letter, const char *name)
{
    if(handle_add(handler, addr, letter, name) == NULL){
    	write(STDERR_FILENO, "Memory allocation failed.\n", 27);
        handle_destructor(handler);
        return (-1);
    }
    return (0);
}

#include <stdio.h>
#include <string.h>

int ft_cmp(t_nmlist *left, t_nmlist *right)
{
    int ret;

    ret = ft_strcmp(left->name, right->name);
    if (ret != 0)
        return (ret);
    return (left->addr >= right->addr);
}

static void ft_swap(t_nmlist **left, t_nmlist **right)
{
	uint8_t buffer[sizeof(t_nmlist *)];

	memcpy(buffer, left, sizeof(*left));
	memcpy(left, right, sizeof(*left));
	memcpy(right, buffer, sizeof(*left));
}

static size_t ft_part(t_nmlist **tab, size_t left, size_t right)
{
    t_nmlist **pivot = &tab[left];
    size_t mleft = left;
    size_t mright = right;

    while (left < right) {
        while (left < mright && ft_cmp(tab[left], pivot[0]) <= 0)
            left++;
        while (right > mleft && ft_cmp(tab[right], pivot[0]) > 0)
            right--;
        if (left < right)
            ft_swap(&tab[left], &tab[right]);
    }
    ft_swap(pivot, &tab[right]);
    return (right);
}

static void ft_qsort(t_nmlist **tab, size_t left, size_t right)
{
	size_t iter;

	if (left >= right)
		return;
    iter = ft_part(tab, left, right);
    if (iter > 0)
	    ft_qsort(tab, left, iter - 1);
	ft_qsort(tab, iter + 1, right);
}

void handle_print(t_nmhandle *handler, char *filename)
{
    t_nmlist *elem = handler->begin;
    t_nmlist **sorted;
    size_t i = 0;

    if (filename)
        printf("\n%s:\n", filename);
    if ((sorted = malloc(handler->current_count * (sizeof(t_nmlist *)))) == NULL) {
        printf("Could not allocate memory to print\n");
        return ;
    }
    for (i = 0; i < handler->current_count; i++) {
        sorted[i] = elem;
        elem = elem->next;
    }
    ft_qsort(sorted, 0, handler->current_count - 1);
    for (i = 0; i < handler->current_count; i++) {
        if (sorted[i]->addr != 0 && ft_strchr("Uw", sorted[i]->letter) == NULL)
			printf("%016lx %c %s\n", sorted[i]->addr, sorted[i]->letter, sorted[i]->name);
		else if (ft_strchr("Uw", sorted[i]->letter) == NULL)
			printf("0000000000000000 %c %s\n", sorted[i]->letter, sorted[i]->name);
		else
			printf("                 %c %s\n", sorted[i]->letter, sorted[i]->name);
    }
    free(sorted);
}

void handle_reset(t_nmhandle *handler)
{
    handler->current = NULL;
    handler->current_count = 0;
}

int handle_constructor(t_nmhandle *handler)
{
    ft_memset(handler, 0, sizeof(*handler));
    for (int i = 0;i<1000;i++) {
        if (handle_add_fof(handler, 0, 0, NULL) == -1)
            return (-1);
    }
    handle_reset(handler);
    handler->alive = 1;
    return (0);
}