/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 17:19:54 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/18 19:47:39 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

bool checkNickname(std::string nickname)
{
	if (nickname.find(" ") == std::string::npos
		&& nickname.find(",") == std::string::npos
		&& nickname.find("*") == std::string::npos
		&& nickname.find("?") == std::string::npos
		&& nickname.find("!") == std::string::npos
		&& nickname.find("@") == std::string::npos
		&& nickname[0] != '$' && nickname[0] != ':'
		&& nickname[0] != '#' && nickname[0] != '&'
		&& nickname.find(".") == std::string::npos)
		return (true);
	return (false);
}

int	IRCServer::_processNick(Message & request, Client & client)
{
	std::vector<std::string> params = request.getParams();
	std::string nickname = params[0];

	if (! client.isLogged())
	{
		client.send(":" + this->_server_name + " " + ERR_PASSWDMISMATCH + " "
				+ client.getNickname() + " :Password incorrect");
		client.send("ERROR :Closing Link: " + client.getNickname()
				+ " (Bad password)");
		client.remove();
		return (1);
	}

	if (params.size() == 0)
		return (client.send(":" + this->_server_name + " " + ERR_NONICKNAMEGIVEN
					+ " " + client.getNickname() + " :No nickname given"));
	else if (params.size() == 1 && checkNickname(nickname))
	{
		if (checkNicknameExist(nickname, this->_clients))
			client.send(":" + this->_server_name + " " + ERR_NICKNAMEINUSE
					+ " " + client.getNickname() + " " + nickname
					+ " :Nickname is already in use.");
		else if (client.getNickname() == "*")
		{
			client.setNickname(params[0]);
			client.send(":" + this->_server_name + " NICK " + client.getNickname());
			if (client.getUsername() != "" && ! client.isRegistered())
			{
				client.setRegistered(true);
				client.send(":" + this->_server_name + " " + RPL_WELCOME + " "
					+ client.getNickname() + " :Welcome to the Internet Relay Network "
					+ client.getFQUN());
			}
		}
		else
		{
			std::string old_nickname = client.getFQUN();
			client.setNickname(params[0]);
			this->_sendToAll(":" + old_nickname + " NICK " + client.getNickname());
		}
	}
	else
		client.send(":" + this->_server_name + " " + ERR_ERRONEUSNICKNAME + " "
				+ client.getNickname() + " " + nickname + " :Erroneus nickname");
	return (0);
}


