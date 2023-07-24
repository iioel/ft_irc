/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 19:34:33 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/24 11:20:18 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int IRCServer::_processWhois(Message & request, Client & client)
{
	std::vector<std::string>	params = request.getParams();
	std::string					target;
	std::string					nick;

	if (params.size() < 1)
	{
		client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
				+ client.getFQUN() + " WHOIS :Not enough parameters");
		return (0);
	}
	else if (params.size() == 1)
		nick = params[0];
	else if (params.size() == 2)
	{
		target = params[0];
		nick = params[1];
	}

	if (!target.empty() && target != this->_server_name)
	{
		client.send(":" + this->_server_name + " " + ERR_NOSUCHSERVER + " "
				+ client.getFQUN() + " " + target + " :No such server");
		return (0);
	}
	Client *target_client = checkNicknameExist(nick, this->_clients);
	if (!target_client)
	{
		client.send(":" + this->_server_name + " " + ERR_NOSUCHNICK + " "
				+ client.getFQUN() + " " + nick + " :No such nick/channel");
		return (0);
	}
	else
	{
		client.send(":" + this->_server_name + " " + RPL_WHOISUSER + " "
				+ client.getFQUN() + " " + target_client->getNickname() + " "
				+ target_client->getUsername() + " " + target_client->getHostname()
				+ " * :" + target_client->getRealname());
		client.send(":" + this->_server_name + " " + RPL_WHOISSERVER + " "
				+ client.getFQUN() + " " + target_client->getNickname() + " "
				+ this->_server_name + " :What an awesome server ! (this is yours)");
		for (std::vector<Channel *>::iterator it = this->_channels.begin();
				it != this->_channels.end(); it++)
		{
			if ((*it)->isMember(target_client))
			{
				std::string rank = (*it)->isChanop(target_client) ? "@" : "";
				client.send(":" + this->_server_name + " " + RPL_WHOISCHANNELS + " "
						+ client.getFQUN() + " " + target_client->getNickname() + " "
						+ rank + (*it)->getName());
			}
		}
		client.send(":" + this->_server_name + " " + RPL_WHOISACTUALLY + " "
				+ client.getFQUN() + " " + target_client->getNickname() + " "
				+ target_client->getUsername() + "@" + target_client->getHostname()
				+ " :Actual user@host");
		client.send(":" + this->_server_name + " " + RPL_ENDOFWHOIS + " "
				+ client.getFQUN() + " " + target_client->getNickname() + " :End of WHOIS list");
	}
	return (0);
}
