/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:16:09 by yoel              #+#    #+#             */
/*   Updated: 2023/07/14 17:09:11 by yoel             ###   ########.fr       */
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
		std::vector<Client *> 		_clients;
		std::vector<std::string> 	_messages;

		void _sendToAll(std::string message);
		void _sendToAllButOne(std::string message, Client *client);

	public:
		Channel(std::string name);
		~Channel();
		Channel(Channel const & src);
		Channel & operator=(Channel const & src);

		// Getters
		std::string getName() const;
		std::vector<Client *> getClients() const;
		std::vector<std::string> getMessages() const;

		// Setters
		void setName(std::string name);
		void setClients(std::vector<Client *> clients);
		void setMessages(std::vector<std::string> messages);

		void addClient(Client *client);
		void removeClient(Client *client);

		void addMessage(std::string message);
		void addMessage(std::string message, Client *client);
};

#endif
