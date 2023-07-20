/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 22:42:09 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/20 14:43:50 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"


int	IRCServer::_processQuit(Message & request, Client & client)
{
	std::vector<std::string>	params = request.getParams();
	if (params.size() > 0)
	{
		this->_sendToAllButOne(":" + client.getFQUN() + " QUIT Quit: " + params[0], client);
		client.send(":" + this->_server_name + " ERROR :Closing Link: "
				+ client.getFQUN() + " (" + params[0] + ")");
		client.remove();
	}
	else
	{
		this->_sendToAllButOne(":" + client.getFQUN() + " QUIT Quit: ", client);
		client.send(":" + this->_server_name + " ERROR :Closing Link: "
				+ client.getFQUN() + " (Client Quit)");
		client.remove();
	}
	return (0);
}
