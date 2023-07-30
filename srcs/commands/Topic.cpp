/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lulutalu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:34:31 by lulutalu          #+#    #+#             */
/*   Updated: 2023/07/30 19:34:00 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processTopic(Message& request, Client& client) {
	std::vector<std::string>	params = request.getParams();

	std::cout << "Params size is " << params.size() << std::endl;
	if (params.size() < 1) {
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS + " " + \
				client.getFQUN() + " TOPIC :Not enough parameters"));
	}

	Channel* chan = checkChannelExist(params[0]);
	if (chan == NULL) {
		return (client.send(":" + _server_name + " " + ERR_NOSUCHCHANNEL + " " + \
					client.getFQUN() + " TOPIC :No such channel"));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + _server_name + " " + ERR_NOTONCHANNEL + " " + \
					client.getFQUN() + " TOPIC :You're not on that channel"));
	}
	if (params.size() == 1) {
		if (chan->getTopic() == "") {
			return (client.send(":" + _server_name + " " + RPL_NOTOPIC + " " + \
						client.getFQUN() + " " + chan->getName() + " :No topic is set"));
		} else {
			client.send(":" + _server_name + " " + RPL_TOPIC + " " + client.getFQUN() + \
					" " + chan->getName() + " :" + chan->getTopic());
			return (client.send(":" + _server_name + " " + RPL_TOPICWHOTIME + \
						" " + client.getFQUN() + " " + chan->getName()+ " " + \
						chan->whoSetTopic()->getNickname() + " " + chan->getStrWhenTopic()));
		}
	} else {
		if (chan->getTopicFlag() && !chan->isChanop(&client)) {
			return (client.send(":" + _server_name + " " + ERR_CHANOPRIVSNEEDED + \
						" " + client.getFQUN() + " " + chan->getName() + \
						" :You're not channel operator"));
		}
		chan->setTopic(params[1]);
		chan->setWhoTopic(&client);
		chan->setWhenTopic(time(NULL));
		chan->sendToAll(":" + _server_name + " " + RPL_TOPIC + " " + chan->getName() + \
				" :" + chan->getTopic());
		chan->sendToAll(":" + _server_name + " " + RPL_TOPICWHOTIME + " " + chan->getName() + \
				chan->whoSetTopic()->getNickname() + " " + chan->getStrWhenTopic());
	}
	return (0);
}
