/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 15:08:33 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/16 22:27:06 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

int	IRCServer::_processCap(Message request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params[0] == "LS")
		client.send(":" + this->_server_name + " CAP * LS :");
	else if (params[0] == "END")
		client.setConnected(true);
	return (0);
}
