/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:07:05 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/30 15:04:55 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processPrivmsg(Message & request, Client & client)
{
	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	targets;
	std::string 				message;
	size_t 						pos;

	if (params.size() == 0)
		return (client.send(":" + this->_server_name + " " + ERR_NORECIPIENT
					+ " " + client.getFQUN() + " :No recipient given (PRIVMSG)"));
	else if (params.size() == 1)
		return (client.send(":" + this->_server_name + " " + ERR_NOTEXTTOSEND
					+ " " + client.getFQUN() + " :No text to send"));

	message = params[1];
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
	}

	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		if (it->find("#") == 0) // Channel
		{
			Channel * target = checkChannelExist(*it);
			if (target)
			{
				if (target->isMember(&client))
					target->sendToAllButOne(":" + client.getFQUN() + " PRIVMSG "
							+ *it + " :" + message, &client);
				else
					client.send(":" + this->_server_name + " " + ERR_CANNOTSENDTOCHAN
							+ " " + client.getFQUN() + " " + *it
							+ " :Cannot send to channel");
			}
			else
				client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL
						+ " " + client.getFQUN() + " " + *it
						+ " :No such channel");
		}
		else // User
		{
			Client *target = client.checkNicknameExist(*it, this->_clients);
			std::cout << "target: " << target << std::endl;
			std::cout << "it: " << *it << std::endl;
			if (target)
			{
				target->send(":" + client.getFQUN() + " PRIVMSG "
						+ *it + " :" + message);
			}
			else
			{
				client.send(":" + this->_server_name + " " + ERR_NOSUCHNICK + " "
						+ client.getFQUN() + " " + *it
						+ " :No such nick");
			}
		}
	}
	return (0);
}


