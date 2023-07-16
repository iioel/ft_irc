/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:09:38 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/16 18:31:00 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

std::string IRCServer::_processUser(Message request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params.size() >= 4)
	{
		std::string realname = params[3].substr(1, params[3].size() - 1);

		for (size_t i = 4; i < params.size(); i++)
			realname += params[i] + " ";

		client.setUsername(params[0]);
		client.setRealname(realname);
		client.setRegistered(true);
		return (":" + this->_server_name + " " + RPL_WELCOME
				+ " " + client.getNickname() + " :Welcome to the Internet Relay Network "
				+ client.getNickname() + "\r\n");
	}
	return (":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " " 
				+ client.getNickname() + " USER :Not enough parameters" + "\r\n");
}
