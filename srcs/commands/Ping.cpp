/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:26:25 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/17 17:33:13 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processPing(Message & request, Client & client)
{
	std::vector<std::string> params = request.getParams();

	if (params.size() == 1)
		return(client.send(":" + this->_server_name + " PONG " + params[0]));

	return (client.send(":" + this->_server_name + ERR_NEEDMOREPARAMS + " "
			+ client.getNickname() + " PING :Not enough parameters"));
}
