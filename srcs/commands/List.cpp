/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 11:51:24 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/30 19:19:28 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int IRCServer::_processList(Message & request, Client & client)
{
	std::vector<std::string>	params = request.getParams();
	std::vector<Channel *>		channels;
	std::vector<std::string>	targets;
	size_t						pos;


	if (params.size() == 0)
		channels = this->_channels;
	else
	{
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
			Channel * channel = checkChannelExist(*it);
			if (! channel)
			{
				client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " "
					+ client.getFQUN() + " " + *it + " :No such channel");
			}
			else
				channels.push_back(channel);
		}
	}

	if (channels.size())
	{
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			std::stringstream			ss;
			ss << (*it)->getMembers().size();
			client.send(":" + this->_server_name + " " + RPL_LIST + " "
				+ client.getFQUN() + " " + (*it)->getName() + " "
				+ ss.str() + " :" + (*it)->getTopic());
		}
	}
	client.send(":" + this->_server_name + " " + RPL_LISTEND + " "
		+ client.getFQUN() + " :End of LIST");
	return (0);
}
