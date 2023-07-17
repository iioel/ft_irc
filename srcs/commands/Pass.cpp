/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 23:19:38 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/18 15:52:42 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processPass(Message & request, Client & client)
{
	std::vector<std::string> params = request.getParams();
	if (params.size() != 1)
	{
		client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
				+ client.getNickname() + " PASS :Not enough parameters");
	}
	else if (! client.isLogged())
	{
			if (params[0] == this->_password)
				client.setLogged(true);
	}
	else
	{
		client.send(":" + this->_server_name + " " + ERR_ALREADYREGISTRED + " "
				+ client.getNickname() + " :Unauthorized command (already registered)");
	}
	return (0);
}
