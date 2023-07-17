/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 15:08:33 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/17 16:32:46 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "Reply.hpp"

int	IRCServer::_processCap(Message request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params.size() == 0)
		client.send(":" + this->_server_name + ERR_NEEDMOREPARAMS
				+ " CAP :Not enough parameters");
	else if (params[0] == "LS")
		client.send(":" + this->_server_name + " CAP * LS :");
	else if (params[0] == "END")
		client.setConnected(true);
	return (0);
}
