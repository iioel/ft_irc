/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 20:19:17 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/24 11:37:24 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processNames(Message & request, Client & client)
{
	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	channels;
	size_t 						pos;

	pos = params[0].find(",");
	if (pos == std::string::npos)
		channels.push_back(params[0]);
	else
	{
		while (pos != std::string::npos)
		{
			channels.push_back(params[0].substr(0, pos));
			params[0].erase(0, pos + 1);
			pos = params[0].find(",");
		}
		channels.push_back(params[0]);
	}

	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = checkChannelExist(*it, this->_channels);
		if (channel)
		{
			std::vector<Client *> members = channel->getMembers();
			for (std::vector<Client *>::iterator it2 = members.begin(); it2 != members.end(); it2++)
			{
				std::string rank = channel->isChanop(*it2) ? "@" : "";
				client.send(":" + this->_server_name + " " + RPL_NAMREPLY + " "
							+ client.getFQUN() + " = " + *it + " :"
							+ rank + (*it2)->getFQUN());
			}
		}
		client.send(":" + this->_server_name + " " + RPL_ENDOFNAMES + " "
					+ client.getFQUN() + " " + *it + " :End of NAMES list");
	}
	return (0);
}
