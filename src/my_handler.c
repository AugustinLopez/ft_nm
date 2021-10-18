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

static int handle_option(char *arg, uint64_t *flag)
{
    int i = 0;
    char err[28] = "Option 'X' not recognized\n\n";

    if (ft_strcmp(arg, "--") == 0)
        return (0);
    else if (arg[0] != '-')
        return (1);
    else if (!arg[1]) {
            err[8] = '-';
            write(STDERR_FILENO, err, 27);
            write(STDERR_FILENO, FT_NM_HELP_MSG, ft_strlen(FT_NM_HELP_MSG));
            return (-1);
    }
    while (arg[++i]) {
        if (arg[i] == 'h') {
            write(STDERR_FILENO, FT_NM_HELP_MSG, ft_strlen(FT_NM_HELP_MSG));
            return (-2);
        }
        else if (arg[i] == 'A' || arg[i] == 'o')
            *flag |= FLAG_FILENAME;
        else if (arg[i] == 'p') {
            *flag &= ~(FLAG_NUMSORT | FLAG_NOSORT);
            *flag |= FLAG_NOSORT;
        }
        else if (arg[i] == 'r') {
            *flag &= ~FLAG_NOSORT;
            *flag |= FLAG_RSORT;
        }
        else if (arg[i] == 'n' || arg[i] == 'v') {
            *flag &= ~FLAG_NOSORT;
            *flag |= FLAG_NUMSORT;
        }
        /*else if (arg[i] == 's')
            *flag |= FLAG_ARMAP;*/
        else if (arg[i] == 'u')
            *flag |= FLAG_UNDEFINED;
        else if (arg[i] == 'g')
            *flag |= FLAG_EXTERNAL;
        else {
            err[8] = arg[i];
            write(STDERR_FILENO, err, 27);
            write(STDERR_FILENO, FT_NM_HELP_MSG, ft_strlen(FT_NM_HELP_MSG));
            return (-1);
        }
    }
    return (1);
}

void handle_reset(t_nmhandle *handler)
{
    handler->current = NULL;
    handler->current_count = 0;
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

int handle_constructor(t_nmhandle *handler, int ac, char **av)
{
    int arg;
    int ret;
    int num = ac - 1;

    ft_memset(handler, 0, sizeof(*handler));
    for (arg = 1; arg < ac; arg++) {
        ret = handle_option(av[arg], &(handler->flag));
        if (ret == -2)
            return (0);
        else if (ret == -1)
            return (-1);
        else if (ret == 0)
            break ;
        else if (av[arg][0] == '-')
            num -= 1;
    }
    handler->flag |= (num > 1) ? FLAG_MULTIPLE_ARG : 0;
    for (int i = 0; i < 1000; i++) {
        if (handle_add_fof(handler, 0, 0, NULL) == -1)
            return (-1);
    }
    handle_reset(handler);
    handler->alive = 1;
    if (num == 0)
        return (1);
    return (2);
}