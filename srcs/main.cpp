/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:24:46 by yoel              #+#    #+#             */
/*   Updated: 2023/07/17 17:37:04 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include "IRCServer.hpp"

int main(int argc, char **argv)
{
	IRCServer ircserv;

	if (argc == 3)
	{
		if (ircserv.init(atoi(argv[1]), argv[2]))
			return (1);
		ircserv.run();
	}
	else
		std::cout << "Usage: ./ft_irc [port] [password]" << std::endl;
	return (0);
}
