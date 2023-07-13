/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:00:52 by yoel              #+#    #+#             */
/*   Updated: 2023/07/14 17:28:30 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name)
{
	;
}

Channel::~Channel()
{
	;
}

Channel::Channel(Channel const & src)
{
	*this = src;
}

Channel & Channel::operator=(Channel const & src)
{
	if (this != &src)
	{
		this->_name = src._name;
		this->_clients = src._clients;
		this->_messages = src._messages;
	}
	return (*this);
}

// Getters

std::string Channel::getName() const
{
	return (this->_name);
}

std::vector<Client *> Channel::getClients() const
{
	return (this->_clients);
}

std::vector<std::string> Channel::getMessages() const
{
	return (this->_messages);
}

// Setters

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setClients(std::vector<Client *> clients)
{
	this->_clients = clients;
}

void Channel::setMessages(std::vector<std::string> messages)
{
	this->_messages = messages;
}

void Channel::addClient(Client *client)
{
	this->_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (*it == client)
		{
			this->_clients.erase(it);
			break ;
		}
		it++;
	}
}

void Channel::addMessage(std::string message)
{
	message = "Server: " + message + "\r\n";
	this->_messages.push_back(message);
	this->_sendToAll(message);
}

void Channel::addMessage(std::string message, Client *client)
{
	message = client->getNickname() + ": " + message + "\r\n";
	this->_messages.push_back(message);
	this->_sendToAllButOne(message, client);
}

void Channel::_sendToAll(std::string message)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		(*it)->setResponse(message);
		it++;
	}
}

void Channel::_sendToAllButOne(std::string message, Client *client)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (*it != client)
			(*it)->setResponse(message);
		it++;
	}
}
