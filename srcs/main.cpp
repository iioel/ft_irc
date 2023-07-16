/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:24:46 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 15:04:45 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstdlib>
#include "IRCServer.hpp"

int main(int argc, char **argv)
{
	IRCServer ircserv;

	if (argc == 4)
	{
		if (ircserv.init(atoi(argv[1]), argv[2]))
			return (1);
		ircserv.run();
	}
	else
		std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
	return (0);
}
