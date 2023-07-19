/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 16:09:40 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/19 19:57:20 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

#include <cstdlib>

std::string	getModes(Channel * channel)
{
	std::string			modes;
	std::string			limit;
	std::stringstream	ss;
	ss << channel->getLimit(); ss >> limit;

	modes = channel->getInviteFlag() ? "i" : "";
	modes += channel->getLimitFlag() ? "l" : "";
	modes += channel->getPasswordFlag() ? "k" : "";
	modes += channel->getTopicFlag() ? "t" : "";
	modes += channel->getLimitFlag() ? " " + limit : "";
	modes += channel->getPasswordFlag() ? " " + channel->getPassword() : "";
	if (modes.size() > 0)
		modes.insert(0, "+");
	return (modes);
}

std::string getModesDiff(Channel * channel, Channel * backup_chan)
{
	std::string			modes;
	std::string			limit;
	std::stringstream	ss;
	ss << channel->getLimit(); ss >> limit;

	if (channel->getInviteFlag() != backup_chan->getInviteFlag())
		modes += channel->getInviteFlag() ? "+i" : "-i";
	if (channel->getLimitFlag() != backup_chan->getLimitFlag())
		modes += channel->getLimitFlag() ? "+l" : "-l";
	if (channel->getPasswordFlag() != backup_chan->getPasswordFlag())
		modes += channel->getPasswordFlag() ? "+k" : "-k";
	if (channel->getTopicFlag() != backup_chan->getTopicFlag())
		modes += channel->getTopicFlag() ? "+t" : "-t";
	if (channel->getLimitFlag() != backup_chan->getLimitFlag())
		modes += channel->getLimitFlag() ? " " + limit : "";
	if (channel->getPasswordFlag() != backup_chan->getPasswordFlag())
		modes += channel->getPasswordFlag() ? " " + channel->getPassword() : "";

	return (modes);
}

int IRCServer::_processMode(Message & message, Client & client)
{
	std::string					target;
	std::string					modes;
	size_t						param_pos;
	bool						sign;

	std::vector<std::string>	params = message.getParams();

	if (params.size() < 1)
		return (client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
				+ client.getNickname() + " MODE :Not enough parameters"));

	target = params[0];
	if (target[0] == '#')
	{
		Channel *channel = checkChannelExist(target, this->_channels);
		Channel backup_chan = *channel;
		if (! channel)
		{
			return (client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " "
					+ client.getNickname() + " " + target + " :No such channel"));
		}
		else if (! channel->isMember(&client))
		{
			return (client.send(":" + this->_server_name + " " + ERR_NOTONCHANNEL + " "
					+ client.getNickname() + " " + target
					+ " :You're not on that channel"));
		}
		else if (params.size() == 1)
		{
			return (client.send(":" + this->_server_name + " " + RPL_CHANNELMODEIS + " "
					+ client.getNickname() + " " + target + " " + getModes(channel)));
		}
		else if (! channel->isChanop(&client) && ! channel->isChancreator(&client))
		{
			return (client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
					+ " " + client.getNickname() + " " + target
					+ " :You're not channel operator"));
		}
		else
		{
			modes = params[1];
			param_pos = 2;
			for (std::string::iterator it = modes.begin(); it != modes.end(); it++)
			{
				if (*it == '+')
					sign = true;
				else if (*it == '-')
					sign = false;
				else if (*it == 'i')
					channel->setInviteFlag(sign);
				else if (*it == 'l')
				{
					if (sign && param_pos < params.size())
					{
						if (params[param_pos].find_first_not_of("0123456789") == std::string::npos)
						{
							channel->setLimitFlag(sign);
							channel->setLimit(std::atoi(params[param_pos].c_str()));
						}
						else
						{
							client.send(":" + this->_server_name + " "
									+ ERR_INVALIDMODEPARAM + " " + client.getNickname()
									+ " " + channel->getName() + " " + *it + " "
									+ params[param_pos]
									+ " :Invalid parameter, should be a number");
						}
						param_pos++;
					}
					else if (sign)
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getNickname()
								+ " MODE :Not enough parameters");
					}
				}
				else if (*it == 'k')
				{
					if (sign && param_pos < params.size())
					{
						channel->setPasswordFlag(sign);
						channel->setPassword(params[param_pos++]);
					}
					else if (sign)
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getNickname()
								+ " MODE :Not enough parameters");
					}
				}
				else if (*it == 't')
					channel->setTopicFlag(sign);
				else if (*it == 'o')
				{
					if (param_pos < params.size())
					{
						Client *client_op = checkNicknameExist(params[param_pos],
								this->_clients);
						if (client_op)
						{
							if (channel->isMember(client_op))
							{
								if (sign)
									channel->addChanop(client_op);
								else
									channel->removeChanop(client_op);
							}
							else
							{
								client.send(":" + this->_server_name
										+ " " + ERR_USERNOTINCHANNEL + " "
										+ client.getNickname() + " "
										+ params[param_pos] + " " + target
										+ " :They aren't on that channel");
							}
						}
						else
						{
							client.send(":" + this->_server_name + " "
									+ ERR_NOSUCHNICK + " " + client.getNickname()
									+ " " + params[param_pos] + " :No such nick");
						}
						param_pos++;
					}
					else
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getNickname()
								+ " MODE :Not enough parameters");
					}
				}
				else
				{
					client.send(":" + this->_server_name + " "
							+ ERR_UNKNOWNMODE + " " + client.getNickname() + " "
							+ *it + " :is unknown mode char to me for " + target);
				}
			}
			client.send(":" + client.getNickname() + " MODE " + target + " "
					+ getModesDiff(channel, &backup_chan));
		}
	}
	else
	{
		if (checkNicknameExist(target, this->_clients))
		{
			if (target != client.getNickname())
			{
				return (client.send(":" + this->_server_name + " "
						+ ERR_USERSDONTMATCH + " " + client.getNickname()
						+ " :Cant change mode for other users"));
			}
		}
		else
		{
			return (client.send(":" + this->_server_name + " " + ERR_NOSUCHNICK + " "
					+ client.getNickname() + " " + target + " :No such nick"));
		}
	}
	return (0);
}
