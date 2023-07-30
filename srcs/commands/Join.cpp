/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduboulo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:42:16 by lduboulo          #+#    #+#             */
/*   Updated: 2023/07/30 15:06:21 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

int	IRCServer::_processJoin(Message & request, Client & client) {

	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	targets;
	std::vector<std::string> 	passwords;
	size_t 						pos;

	if (params.size() < 1)
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS
				+ client.getFQUN() + " JOIN :Not enough parameters"));

	// Parse channel names
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

	// Parse passwords
	if (params.size() == 2)
	{
		pos = params[1].find(",");
		if (pos == std::string::npos)
			passwords.push_back(params[1]);
		else
		{
			while (pos != std::string::npos)
			{
				passwords.push_back(params[1].substr(0, pos));
				params[1].erase(0, pos + 1);
				pos = params[1].find(",");
			}
			passwords.push_back(params[1]);
		}
	}

	// Iterate over channels
	std::vector<std::string>::iterator it2 = passwords.begin();
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		std::string channel_name = *it;

		if (channel_name[0] != '#' // Check channel name
				|| channel_name.find_first_of(",: \n\r") != std::string::npos)
		{
			client.send(":" + _server_name + " " + ERR_BADCHANMASK + " "
					+ client.getFQUN() + " " + params[0] + " :Bad Channel Mask");
			continue ;
		}

		Channel *channel = checkChannelExist(channel_name);
		if (channel == NULL)
		{
			if (it2 != passwords.end()) // Create new channel with pass
				channel = new Channel(channel_name, *it2);
			else // Create new channel
				channel = new Channel(channel_name);
			this->_channels.push_back(channel);
			channel->addChancreator(&client);
			channel->addChanop(&client);
			channel->addMember(&client);
		}
		else if (channel->getPasswordFlag() // Password protected channel
				&& (it2 == passwords.end() || channel->getPassword() != *it2))
		{
			client.send(":" + _server_name + " " + ERR_BADCHANNELKEY
					+ client.getFQUN() + " " + params[0] + " :Wrong Channel Key");
			continue ;
		}
		else if (channel->getLimitFlag() // Channel limit reached
				&& static_cast<size_t>(channel->getLimit()) <= channel->getMembers().size())
		{
			client.send(":" + _server_name + " " + ERR_CHANNELISFULL + " "
					+ client.getFQUN() + " " + params[0] + " :Channel is full");
			continue ;
		}
		else if (channel->isMember(&client)) // Already on channel
		{
			client.send(":" + _server_name + " " + ERR_USERONCHANNEL + " "
					+ client.getFQUN() + " " + params[0]
					+ " :You are already on that channel");
			continue ;
		}
		else if (channel->getInviteFlag() && !channel->isInvited(&client)) // Invite only
		{
			client.send(":" + _server_name + " " + ERR_INVITEONLYCHAN + " "
					+ client.getFQUN() + " " + params[0]
					+ " :Cannot join channel (+i)");
			continue ;
		}
		else
			channel->addMember(&client);
		channel->sendToAll(":" + client.getFQUN() + " JOIN " + channel_name);
		client.send(":" + this->_server_name + " " + RPL_CREATIONTIME + " "
					+ client.getFQUN() + " " + channel_name + " "
					+ channel->getStrCreationTime());
		client.send(":" + this->_server_name + " " + RPL_TOPIC + " "
					+ client.getFQUN() + " " + channel_name + " :"
					+ channel->getTopic());
		client.send(":" + this->_server_name + " " + RPL_CHANNELMODEIS + " "
					+ client.getFQUN() + " " + channel_name + " "
					+ channel->getModes());
		std::vector<Client *> members = channel->getMembers();
		for (std::vector<Client *>::iterator it3 = members.begin(); it3 != members.end(); it3++)
		{
			std::string rank;
			if (channel->isChanop(*it3))
				rank = "@";
			else if (channel->isChancreator(*it3))
				rank = "&";
			client.send(":" + this->_server_name + " " + RPL_NAMREPLY + " "
						+ client.getFQUN() + " = " + channel_name + " :"
						+ rank + (*it3)->getFQUN());
		}
		client.send(":" + this->_server_name + " " + RPL_ENDOFNAMES + " "
					+ client.getFQUN() + " " + channel_name + " :End of NAMES list");

		if (it2 != passwords.end())
			it2++;
	}
	return (0);
}
