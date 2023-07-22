/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduboulo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/22 14:56:28 by lulutalu          #+#    #+#             */
/*   Updated: 2023/07/22 15:32:36 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCServer.hpp"
#include "../../includes/Reply.hpp"

/*	
 *	Command to kick one or multiple user from a channel
 *	The operator of the command need to have moderator acces to perform this action
 *	The operator need to be in the same channel
*/

int	IRCServer::_processKick(Message &request, Client &client) {
	std::vector<std::string>	params = request.getParams();
	size_t						nSep = 0;

	if (params.size() < 2) {
		return (client.send(":" + this->_server_name + " " + ERR_NEEDMOREPARAMS + \
					client.getFQUN() + " KICK :Not enough parameters"));
	}



}
