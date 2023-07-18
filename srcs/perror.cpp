/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perror.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:22:32 by yoel              #+#    #+#             */
/*   Updated: 2023/07/18 19:44:47 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <errno.h>
#include "perror.hpp"

int perror(std::string str)
{
	std::cout << str << ": " << strerror(errno) << std::endl;
	return (1);
}
