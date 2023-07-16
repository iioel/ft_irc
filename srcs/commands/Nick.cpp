/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 17:19:54 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/16 18:29:34 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

std::string IRCServer::_processNick(Message request, Client & client)
{
	std::vector<std::string> params = request.getParams();

	if (params.size() == 1)
	{
		if (client.getNickname().empty())
		{
			client.setNickname(params[0]);
		}
		else
		{
			client.setNickname(params[0]);
		}
	}
	return (":" + this->_server_name + " " ERR_NONICKNAMEGIVEN + " "
				+ client.getNickname() + " :No nickname given" + "\r\n");
}

