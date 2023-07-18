/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:16:09 by yoel              #+#    #+#             */
/*   Updated: 2023/07/19 15:37:28 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include "Client.hpp"

class Channel
{
	private:
		std::string 				_name;
		std::string					_password;
		bool						_isInviteOnly;
		std::vector<Client *>		_moderators;
		std::vector<Client *> 		_clients;

	public:
		Channel(std::string name, std::string password);
		~Channel();
		Channel(Channel const & src);
		Channel & operator=(Channel const & src);

		// Getters
		std::string 			getName() const;
		std::vector<Client *> 	getClients() const;
		std::vector<Client *> 	getModerators() const;
		std::string				getPassword() const;
		bool 					isChannelInviteOnly() const;

		// Setters
		void setName(std::string name);
		void setPassword(std::string password);
		void setClients(std::vector<Client *> clients);
		void setModerators(std::vector<Client *> moderators);
		void setIsChannelInviteOnly(bool isInviteOnly);

		void addClient(Client *client);
		void removeClient(Client *client);

		void addModerator(Client *moderator);
		void removeModerator(Client *moderator);

		void sendToAll(std::string message);
		void sendToAllButOne(std::string message, Client *client);

};

Channel * 	checkChannelExist(std::string channelName, std::vector<Channel *> channels);
bool		checkClientInChannel(Client * client, Channel * channel);

#endif
