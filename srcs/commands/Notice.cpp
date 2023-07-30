/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 19:07:05 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/30 19:18:21 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processNotice(Message & request, Client & client)
{
	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	targets;
	std::string 				message;
	size_t 						pos;

	if (params.size() == 0)
		return (0);
	else if (params.size() == 1)
		return (0);

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
		targets.push_back(params[0]);
	}

	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		if (it->find("#") == 0) // Channel
		{
			Channel * target = checkChannelExist(*it);
			if (target)
			{
				if (target->isMember(&client))
					target->sendToAllButOne(":" + client.getFQUN() + " NOTICE "
							+ *it + " :" + message, &client);
			}
		}
		else // User
		{
			Client *target = client.checkNicknameExist(*it, this->_clients);
			if (target)
			{
				target->send(":" + client.getFQUN() + " NOTICE "
						+ *it + " :" + message);
			}
		}
	}
	return (0);
}
