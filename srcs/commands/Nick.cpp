/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 17:19:54 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/16 21:10:22 by ycornamu         ###   ########.fr       */
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

std::string IRCServer::_processNick(Message request, Client & client)
{
	std::string reply = "";

	std::vector<std::string> params = request.getParams();
	if (params.size() == 0)
		return (":" + this->_server_name + " " + ERR_NONICKNAMEGIVEN + " " \
				+ client.getNickname() + " :No nickname given" + "\r\n");

	std::string nickname;
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		nickname += *it;

	std::cout << "nickname: " << nickname << std::endl;

	if (params.size() == 1 && checkNickname(nickname))
	{
		if (checkNicknameExist(nickname, this->_clients))
			return (":" + this->_server_name + " " + ERR_NICKNAMEINUSE + " " \
					+ client.getNickname() + " " + nickname + "\r\n");
		if (client.getNickname().empty())
		{
			client.setNickname(params[0]);
			if (client.getUsername() != "" && ! client.isRegistered())
			{
				client.setRegistered(true);
				reply = ":" + this->_server_name + " " + RPL_WELCOME + " "
					+ client.getNickname() + " :Welcome to the Internet Relay Network "
					+ client.getNickname() + "\r\n";
			}
			return (reply + ":" + this->_server_name + " NICK "
					+ client.getNickname() + "\r\n");
		}
		else
		{
			std::string old_nickname = client.getNickname();
			client.setNickname(params[0]);
			_sendToAll(":" + old_nickname + " NICK " + client.getNickname() + "\r\n");
			return ("");
		}
	}
	else
		return (":" + this->_server_name + " " + ERR_ERRONEUSNICKNAME + " " \
				+ client.getNickname() + " " + nickname
				+ " :Erroneus nickname" + "\r\n");
	return ("");
}

