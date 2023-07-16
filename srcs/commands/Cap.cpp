/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 15:08:33 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/16 16:11:11 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

std::string	IRCServer::_processCap(Message request, Client & client)
{
	std::vector<std::string>	params = request.getParams();

	if (params[0] == "LS")
		return ("CAP * LS :\r\n");
	if (params[0] == "END")
	{
		client.setConnected(true);
		return ("");
	}
	return ("");
}
