/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lulutalu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 16:58:52 by lulutalu          #+#    #+#             */
/*   Updated: 2023/07/30 17:31:44 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processInvite(Message& request, Client& client) {
	std::vector<std::string>	params = request.getParams();

	if (params.size() != 2) {
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS + \
					client.getFQUN() + " INVITE :Not enough parameters"));
	}

	Channel* chan = checkChannelExist(params[1]);
	if (chan == NULL) {
		return (client.send(":" + _server_name + " " + ERR_NOSUCHCHANNEL + \
					client.getFQUN() + " INVITE :Target channel doesn't exist"));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + _server_name + " " + ERR_NOTONCHANNEL + \
					client.getFQUN() + " INVITE :You're not on that channel"));
	}
	if (chan->getInviteFlag() && !chan->isChanop(&client)) {
		return (client.send(":" + _server_name + " " + ERR_CHANOPRIVSNEEDED + \
					client.getFQUN() + " INVITE :You're not a channel operator"));
	}
	if (chan->isMember(chan->getClientFromNickname(params[0]))) {
		return (client.send(":" + _server_name + " " + ERR_USERONCHANNEL + \
					client.getFQUN() + " INVITE :Is already on channel"));
	}

	Client*	user = this->findClientByNickname(params[0]);
	if (user != NULL) {
		chan->addInvited(user);
		client.send(":" + _server_name + " " + RPL_INVITING + " " + \
				client.getFQUN() + " INVITE :You've invited " + user->getNickname() + \
				" to the channel " + chan->getName());
		user->send(":" + _server_name + " " + user->getFQUN() + \
				" INVITE: You've been invited to " + chan->getName() + " by " + \
				client.getNickname());
	}
	return (0);
}
