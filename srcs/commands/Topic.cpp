/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lulutalu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:34:31 by lulutalu          #+#    #+#             */
/*   Updated: 2023/07/31 16:46:43 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processTopic(Message& request, Client& client) {
	std::vector<std::string>	params = request.getParams();

	Channel* chan = checkChannelExist(params[0]);
	if (chan == NULL) {
		return (client.send(":" + _server_name + " " + ERR_NOSUCHCHANNEL + " " + \
					client.getFQUN() + " TOPIC "));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + _server_name + " " + ERR_NOTONCHANNEL + " " + \
					client.getFQUN() + " TOPIC: You're not on that channel"));
	}
	if (chan->getTopicFlag() && !chan->isChanop(&client)) {
		return (client.send(":" + _server_name + " " + ERR_CHANOPRIVSNEEDED + \
					" " + client.getFQUN() + " TOPIC: You're not channel operator"));
	} else {
		chan->setTopic(params[1]);
		chan->setWhoTopic(&client);
		chan->setWhenTopic(time(0));
		chan->sendToAll(client.getFQUN() + " TOPIC " + chan->getName() + " :" + chan->getTopic());
	}
	return (0);
}
