/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 18:47:49 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/24 12:27:07 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int IRCServer::_processWho(Message & request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params.size() == 0)
	{
		client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
			+ client.getFQUN() + " WHO :Not enough parameters");
		return (0);
	}

	std::string		mask = params[0];

	if (mask[0] == '#')
	{
		Channel *channel = checkChannelExist(mask, this->_channels);
		if (channel)
		{
			std::vector<Client *>	members = channel->getMembers();
			for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); it++)
			{
				std::string rank = channel->isChanop(*it) ? "@" : "";
				client.send(":" + this->_server_name + " " + RPL_WHOREPLY + " "
					+ client.getFQUN() + " " + channel->getName() + " "
					+ (*it)->getUsername() + " " + (*it)->getHostname() + " "
					+ this->_server_name + " " + (*it)->getNickname() + " H"
					+ rank + " :0 " + (*it)->getRealname());
			}
			client.send(":" + this->_server_name + " " + RPL_ENDOFWHO + " "
				+ client.getFQUN() + " " + channel->getName() + " :End of WHO list");
		}
		else
		{
			client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " "
				+ client.getFQUN() + " " + mask + " :No such channel");
		}
	}
	else if (mask.find(".") != std::string::npos)
	{
		client.send(":" + this->_server_name + " " + ERR_NOSUCHSERVER + " "
			+ client.getFQUN() + " " + mask + " :No such server");
	}
	else
	{
		Client *target = checkNicknameExist(mask, this->_clients);
		if (target)
		{
			client.send(":" + this->_server_name + " " + RPL_WHOREPLY + " "
				+ client.getFQUN() + " * " + target->getUsername() + " "
				+ target->getHostname() + " " + this->_server_name + " "
				+ target->getNickname() + " H :0 " + target->getRealname());
			client.send(":" + this->_server_name + " " + RPL_ENDOFWHO + " "
				+ client.getFQUN() + " " + target->getNickname() + " :End of WHO list");
		}
		else
		{
			client.send(":" + this->_server_name + " " + ERR_NOSUCHNICK + " "
				+ client.getFQUN() + " " + mask + " :No such nick/channel");
		}
	}
	return (0);
}
