/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:09:38 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/17 17:26:01 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processUser(Message & request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params.size() == 4)
	{
		client.setUsername(params[0]);
		client.setRealname(params[3]);

		if (client.getNickname() != "" && ! client.isRegistered())
		{
			client.setRegistered(true);
			client.send(":" + this->_server_name + " " + RPL_WELCOME + " "
				+ client.getNickname() + " :Welcome to the Internet Relay Network "
				+ client.getNickname());
		}
	}
	else
		client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
			+ client.getNickname() + " USER :Not enough parameters");
	return (0);
}
