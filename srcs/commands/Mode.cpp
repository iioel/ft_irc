/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 16:09:40 by ycornamu          #+#    #+#             */
/*   Updated: 2023/08/12 17:10:02 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

#include <cstdlib>

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
	if (channel->getChanops().size() > backup_chan->getChanops().size())
		modes += "+o";
	else if (channel->getChanops().size() < backup_chan->getChanops().size())
		modes += "-o";
	if (channel->getTopicFlag() != backup_chan->getTopicFlag())
		modes += channel->getTopicFlag() ? "+t" : "-t";
	if (channel->getLimitFlag() != backup_chan->getLimitFlag())
		modes += channel->getLimitFlag() ? " " + limit : "";
	if (channel->getPasswordFlag() != backup_chan->getPasswordFlag())
		modes += channel->getPasswordFlag() ? " " + channel->getPassword() : "";
	if (channel->getChanops().size() > backup_chan->getChanops().size())
	{
		std::vector<Client *>	chanops = channel->getChanops();
		for (std::vector<Client *>::iterator it = chanops.begin();
				it != chanops.end(); it++)
		{
			if (! backup_chan->isChanop(*it))
				modes += " " + (*it)->getNickname();
		}
	}
	else if (channel->getChanops().size() < backup_chan->getChanops().size())
	{
		std::vector<Client *>	chanops = backup_chan->getChanops();
		for (std::vector<Client *>::iterator it = chanops.begin();
				it != chanops.end(); it++)
		{
			if (! channel->isChanop(*it))
				modes += " " + (*it)->getNickname();
		}
	}

	return (modes);
}

int IRCServer::_processMode(Message & message, Client & client)
{
	std::string					target;
	std::string					modes;
	size_t						param_pos;
	bool						sign;
	bool						showModeDiff = false;

	std::vector<std::string>	params = message.getParams();

	if (params.size() < 1)
		return (client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + " "
				+ client.getFQUN() + " MODE :Not enough parameters"));

	target = params[0];
	if (target[0] == '#')
	{
		Channel *channel = checkChannelExist(target);
		Channel backup_chan = *channel;
		if (! channel)
		{
			return (client.send(":" + this->_server_name + " " + ERR_NOSUCHCHANNEL + " "
					+ client.getFQUN() + " " + target + " :No such channel"));
		}
		else if (! channel->isMember(&client))
		{
			return (client.send(":" + this->_server_name + " " + ERR_NOTONCHANNEL + " "
					+ client.getFQUN() + " " + target
					+ " :You're not on that channel"));
		}
		else if (params.size() == 1)
		{
			return (client.send(":" + this->_server_name + " " + RPL_CHANNELMODEIS + " "
					+ client.getFQUN() + " " + target + " " + channel->getModes()));
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
				{
					showModeDiff = true;
					if (! channel->isChanop(&client))
					{
						 client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
								+ " " + client.getFQUN() + " " + target
								+ " :You're not channel operator");
						 continue;
					}
					channel->setInviteFlag(sign);
				}
				else if (*it == 'l')
				{
					showModeDiff = true;
					if (! channel->isChanop(&client))
					{
						 client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
								+ " " + client.getFQUN() + " " + target
								+ " :You're not channel operator");
						 continue;
					}
					else if ( sign && param_pos < params.size())
					{
						if (params[param_pos].find_first_not_of("0123456789") == std::string::npos)
						{
							channel->setLimitFlag(sign);
							if (sign)
								channel->setLimit(std::atoi(params[param_pos].c_str()));
						}
						else
						{
							client.send(":" + this->_server_name + " "
									+ ERR_INVALIDMODEPARAM + " " + client.getFQUN()
									+ " " + channel->getName() + " " + *it + " "
									+ params[param_pos]
									+ " :Invalid parameter, should be a number");
						}
						param_pos++;
					}
					else if (sign)
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getFQUN()
								+ " MODE :Not enough parameters");
					}
					else
						channel->setLimitFlag(sign);
				}
				else if (*it == 'k')
				{
					showModeDiff = true;
					if (! channel->isChanop(&client))
					{
						 client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
								+ " " + client.getFQUN() + " " + target
								+ " :You're not channel operator");
						 continue;
					}
					else if (sign && param_pos < params.size())
					{
						channel->setPasswordFlag(sign);
						if (sign)
							channel->setPassword(params[param_pos++]);
					}
					else if (sign)
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getFQUN()
								+ " MODE :Not enough parameters");
					}
					else
						channel->setPasswordFlag(sign);
				}
				else if (*it == 't')
				{
					showModeDiff = true;
					if (! channel->isChanop(&client))
					{
						 client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
								+ " " + client.getFQUN() + " " + target
								+ " :You're not channel operator");
						 continue;
					}
					channel->setTopicFlag(sign);
				}
				else if (*it == 'o')
				{
					showModeDiff = true;
					if (! channel->isChanop(&client))
					{
						 client.send(":" + this->_server_name + " " + ERR_CHANOPRIVSNEEDED
								+ " " + client.getFQUN() + " " + target
								+ " :You're not channel operator");
						 continue;
					}
					else if (param_pos < params.size())
					{
						Client *client_op = client.checkNicknameExist(params[param_pos],
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
										+ client.getFQUN() + " "
										+ params[param_pos] + " " + target
										+ " :They aren't on that channel");
							}
						}
						else
						{
							client.send(":" + this->_server_name + " "
									+ ERR_NOSUCHNICK + " " + client.getFQUN()
									+ " " + params[param_pos] + " :No such nick");
						}
						param_pos++;
					}
					else
					{
						client.send(":" + this->_server_name + " "
								+ ERR_NEEDMOREPARAMS + " " + client.getFQUN()
								+ " MODE :Not enough parameters");
					}
				}
				else if (*it == 'b') {
					client.send(":" + this->_server_name + " " + RPL_ENDOFBANLIST + " " +
							client.getFQUN() + " " + target + " :End of ban list");
				}
				else
				{
					return (client.send(":" + this->_server_name + " "
							+ ERR_UNKNOWNMODE + " " + client.getFQUN() + " "
							+ *it + " :is unknown mode char to me for " + target));
				}
			}
			if (showModeDiff)
			{
				client.send(":" + client.getFQUN() + " MODE " + target + " "
						+ getModesDiff(channel, &backup_chan));
				if (getModesDiff(channel, &backup_chan) != "")
					channel->sendToAllButOne(":" + client.getFQUN() + " MODE " + target + " "
							+ getModesDiff(channel, &backup_chan), &client);
			}
		}
	}
	else
	{
		if (client.checkNicknameExist(target, this->_clients))
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
					+ client.getFQUN() + " " + target + " :No such nick"));
		}
	}
	return (0);
}
