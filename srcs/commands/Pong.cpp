/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 18:05:17 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/20 14:45:35 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int IRCServer::_processPong(Message & request, Client & client)
{
	std::vector<std::string> params = request.getParams();

	if (params.size() == 1)
	{
		if (client.getPingStr() == params[0])
			client.setPong(true);
		return (0);
	}

	return (client.send(":" + this->_server_name + ERR_NOORIGIN + " "
			+ client.getFQUN() + " :No origin specified"));
}
