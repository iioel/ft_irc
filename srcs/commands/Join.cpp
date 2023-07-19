/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduboulo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 18:42:16 by lduboulo          #+#    #+#             */
/*   Updated: 2023/07/19 21:48:42 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int	IRCServer::_processJoin(Message & request, Client & client) {

	std::vector<std::string> 	params = request.getParams();
	std::vector<std::string> 	targets;
	std::vector<std::string> 	passwords;
	size_t 						pos;

	if (params.size() < 1)
		return (client.send(":" + _server_name + " " + ERR_NEEDMOREPARAMS
				+ client.getNickname() + " JOIN :Not enough parameters"));

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
		}
	}

	// Iterate over channels
	std::vector<std::string>::iterator it2 = passwords.begin();
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); it++)
	{
		std::string channel_name = *it;

		if (channel_name[0] != '#' // Check channel name
				|| channel_name.find_first_of(",: \n\r") != std::string::npos)
			return (client.send(":" + _server_name + " " + ERR_BADCHANMASK
					+ client.getNickname() + " " + params[0] + " :Bad Channel Mask"));

		Channel *channel = checkChannelExist(channel_name, this->_channels);
		if (channel == NULL && it2 != passwords.end()) // Create new channel with pass
		{
			channel = new Channel(channel_name, *it2);
			this->_channels.push_back(channel);
			channel->addChancreator(&client);
			channel->addMember(&client);
			channel->sendToAll(":" + client.getNickname() + " JOIN " + channel_name);
		}
		else if (channel == NULL) // Create new channel
		{
			channel = new Channel(channel_name);
			this->_channels.push_back(channel);
			channel->addChancreator(&client);
			channel->addMember(&client);
			channel->sendToAll(":" + client.getNickname() + " JOIN " + channel_name);
		}
		else if (channel->getPasswordFlag() // Password protected channel
				&& (it2 == passwords.end() || channel->getPassword() != *it2))
			return (client.send(":" + _server_name + " " + ERR_BADCHANNELKEY
					+ client.getNickname() + " " + params[0] + " :Wrong Channel Key"));
		else if (channel->getLimitFlag() // Channel limit reached
				&& static_cast<size_t>(channel->getLimit()) <= channel->getMembers().size())
			return (client.send(":" + _server_name + " " + ERR_CHANNELISFULL
					+ client.getNickname() + " " + params[0] + " :Channel is full"));
		else if (channel->isMember(&client)) // Already on channel
			return (client.send(":" + _server_name + " " + ERR_USERONCHANNEL
					+ client.getNickname() + " " + params[0]
					+ " :You are already on that channel"));
		else if (channel->getInviteFlag() && !channel->isInvited(&client)) // Invite only
			return (client.send(":" + _server_name + " " + ERR_INVITEONLYCHAN
					+ client.getNickname() + " " + params[0]
					+ " :Cannot join channel (+i)"));
		else
		{
			channel->addMember(&client);
			channel->sendToAll(":" + client.getNickname() + " JOIN " + channel_name);
		}
		if (it2 != passwords.end())
			it2++;
	}
	return (0);
}
