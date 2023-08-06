/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lulutalu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 16:58:52 by lulutalu          #+#    #+#             */
/*   Updated: 2023/08/06 19:00:59 by lduboulo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processInvite(Message& request, Client& client) {
	std::vector<std::string>	params = request.getParams();

	if (params.size() != 2) {
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS + " " +
					client.getFQUN() + " INVITE :Not enough parameters"));
	}

	Channel* chan = checkChannelExist(params[1]);
	if (chan == NULL) {
		return (client.send(":" + _server_name + " " + ERR_NOSUCHCHANNEL + " " +
					client.getFQUN() + " " + params[1] + " :No such channel"));
	}
	if (!chan->isMember(&client)) {
		return (client.send(":" + _server_name + " " + ERR_NOTONCHANNEL + " " +
					client.getFQUN() + " " + params[1] + " :You're not on that channel"));
	}
	if (chan->getInviteFlag() && !chan->isChanop(&client)) {
		return (client.send(":" + _server_name + " " + ERR_CHANOPRIVSNEEDED + " " +
					client.getFQUN() + " " + params[1] + " :You're not channel operator"));
	}
	if (chan->isMember(chan->getClientFromNickname(params[0]))) {
		return (client.send(":" + _server_name + " " + ERR_USERONCHANNEL + " " +
					client.getFQUN() + " " + params[0] + " " + params[1] + " :is already on channel"));
	}

	Client*	user = this->findClientByNickname(params[0]);
	if (user != NULL) {
		client.send(":" + _server_name + " " + RPL_INVITING + " " + client.getFQUN() +
				" " + params[0] + " " + params[1]);
		user->send(":" + client.getFQUN() + " INVITE " + params[0] + " " + params[1]);
		chan->addInvited(user);
	}
	return (0);
}
