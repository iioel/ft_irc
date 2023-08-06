/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lulutalu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:34:31 by lulutalu          #+#    #+#             */
/*   Updated: 2023/08/06 15:56:23 by lduboulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processTopic(Message& request, Client& client) {
	std::vector<std::string>	params = request.getParams();

	if (params.size() < 1) {
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS + " " +
					client.getFQUN() + " TOPIC :Not enough parameters"));
	}
	Channel* chan = checkChannelExist(params[0]);
	if (chan == NULL) {
		return (client.send(":" + _server_name + " " + ERR_NOSUCHCHANNEL + " " + \
					client.getFQUN() + " " + params[0] + " :No such channel"));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + _server_name + " " + ERR_NOTONCHANNEL + " " + \
					client.getFQUN() + " " + params[0] + " :You're not on that channel"));
	}
	if (chan->getTopicFlag() && !chan->isChanop(&client)) {
		return (client.send(":" + _server_name + " " + ERR_CHANOPRIVSNEEDED + \
					" " + client.getFQUN() + " " + params[0] + " :You're not channel operator"));
	} else {
		chan->setTopic(params[1]);
		chan->setWhoTopic(&client);
		chan->setWhenTopic(time(0));
		chan->sendToAll(":" + client.getFQUN() + " TOPIC " + chan->getName() + " :" + chan->getTopic());
	}
	return (0);
}
