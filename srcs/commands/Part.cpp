/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:34:45 by yoel              #+#    #+#             */
/*   Updated: 2023/07/21 12:05:21 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processPart(Message & request, Client & client)
{
	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	targets;
	size_t 						pos;	
	std::string 				reason;

	if (params.size() < 1)
	{
		return(client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
				+ client.getFQUN() + " PART :Not enough parameters"));
	}

	if (params.size() > 1)
	{
		reason = params[1];
		for (size_t i = 2; i < params.size(); i++)
			reason += params[i] + " ";
	}

	pos = params[0].find(",");
	if (pos == std::string::npos)
		targets.push_back(params[0]);
	else
	{
		while (pos != std::string::npos)
		{
			targets.push_back(params[0].substr(0, pos));
			params[0].erase(0, pos + 1);
			pos = params[0].find(",");
		}
		targets.push_back(params[0]);
	}
	
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		Channel * channel = checkChannelExist(*it, this->_channels);
		if (! channel)
		{
			client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " "
					+ client.getFQUN() + " " + *it + " :No such channel");
		}
		else if (! channel->isMember(&client))
		{
			client.send(":" + this->_server_name + " " + ERR_NOTONCHANNEL + " "
					+ client.getFQUN() + " " + *it + " :You're not on that channel");
		}
		else
		{
			channel->sendToAll(":" + client.getFQUN() + " PART "
					+ channel->getName() + " :" + reason);
			channel->removeMember(&client);
			if (channel->getMembers().size() == 0)
				this->_removeChannel(channel);
		}
	}
	return (0);
}
