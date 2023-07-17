/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 17:19:54 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/17 17:25:51 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

bool checkNickname(std::string nickname)
{
	if (! std::strchr(nickname.c_str(), '#')
		&& ! std::strchr(nickname.c_str(), '&')
		&& ! std::strchr(nickname.c_str(), ':')
		&& ! std::strchr(nickname.c_str(), ' '))
		return (true);
	return (false);
}

bool checkNicknameExist(std::string nickname, std::vector<Client> clients)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getNickname() == nickname)
			return (true);
	}
	return (false);
}

int	IRCServer::_processNick(Message & request, Client & client)
{
	std::vector<std::string> params = request.getParams();
	if (params.size() == 0)
		return (client.send(":" + this->_server_name + " " + ERR_NONICKNAMEGIVEN
					+ " " + client.getNickname() + " :No nickname given"));

	std::string nickname;
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		nickname += *it;

	if (params.size() == 1 && checkNickname(nickname))
	{
		if (checkNicknameExist(nickname, this->_clients))
			client.send(":" + this->_server_name + " " + ERR_NICKNAMEINUSE
					+ " " + client.getNickname() + " " + nickname);
		else if (client.getNickname().empty())
		{
			client.setNickname(params[0]);
			if (client.getUsername() != "" && ! client.isRegistered())
			{
				client.setRegistered(true);
				client.send(":" + this->_server_name + " " + RPL_WELCOME + " "
					+ client.getNickname() + " :Welcome to the Internet Relay Network "
					+ client.getNickname());
			}
			client.send(":" + this->_server_name + " NICK " + client.getNickname());
		}
		else
		{
			std::string old_nickname = client.getNickname();
			client.setNickname(params[0]);
			_sendToAll(":" + old_nickname + " NICK " + client.getNickname());
		}
	}
	else
		client.send(":" + this->_server_name + " " + ERR_ERRONEUSNICKNAME + " "
				+ client.getNickname() + " " + nickname + " :Erroneus nickname");
	return (0);
}


