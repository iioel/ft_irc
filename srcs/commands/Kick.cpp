/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduboulo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 14:56:28 by lulutalu          #+#    #+#             */
/*   Updated: 2023/08/06 18:43:06 by lduboulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Reply.hpp"

/*	
 *	Command to kick one or multiple user from a channel
 *	The operator of the command need to have moderator acces to perform this action
 *	The operator need to be in the same channel
*/

int	IRCServer::_processKick(Message &request, Client &client) {
	std::vector<std::string>	params = request.getParams();
	std::vector<std::string>	target_users;
	size_t						nSep = 0;

	if (params.size() < 2) {
		return (client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " " +
					client.getFQUN() + " KICK :Not enough parameters"));
	}

	Channel* chan = this->checkChannelExist(params[0]);
	if (chan == NULL) {
		return (client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " " +
					client.getFQUN() + " " + params[0] + " :No such channel"));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + this->_server_name + " " + ERR_NOTONCHANNEL + " " +
					client.getFQUN() + " " + params[0] + " :You're not on that channel"));
	}
	if (!chan->isChanop(&client)) {
		return (client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED + " " +
					client.getFQUN() + " " + params[0] + " :You're not channel operator"));
	}

	nSep = params[1].find(",");
	if (nSep == std::string::npos) {
		target_users.push_back(params[1]);
	} else {
		while (nSep != std::string::npos) {
			target_users.push_back(params[1].substr(0, nSep));
			params[1].erase(0, nSep + 1);
			nSep = params[1].find(",");
		}
		target_users.push_back(params[1]);
	}

	for (std::vector<std::string>::iterator it = target_users.begin();
			it != target_users.end(); it++) {
		Client* user = chan->getClientFromNickname(*it);
		if (user == NULL) {
			return (client.send(":" + this->_server_name + " " + ERR_USERNOTINCHANNEL + " " +
						client.getFQUN() + " " + *it + " " + chan->getName() +
						" :They aren't on that channel"));
		} else if (params.size() == 3 && !params[2].empty()) {
			chan->sendToAll(":" + client.getFQUN() + " KICK " + chan->getName() + " " + *it + " :" + params[2]);
			chan->removeMember(user);
		} else {
			chan->sendToAll(":" + client.getFQUN() + " KICK " + chan->getName() + " " + *it + " :Infringement of rules");
			chan->removeMember(user);
		}
	}
	return (0);
}
