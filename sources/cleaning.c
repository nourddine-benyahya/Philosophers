/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenyahy <nbenyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:29:36 by nbenyahy          #+#    #+#             */
/*   Updated: 2024/05/06 19:23:09 by nbenyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void exit_with_msg(char *msg, int status)
{
    if (msg)
        printf("%s", msg);
    exit(status);
}