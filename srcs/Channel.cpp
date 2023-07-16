/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:00:52 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 22:32:02 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(std::string name, std::string password) : _name(name), _password(password)
{
	this->_isInviteOnly = false;
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

std::vector<Client *> Channel::getModerators() const {
	return this->_moderators;
}

std::vector<std::string> Channel::getMessages() const
{
	return (this->_messages);
}

std::string Channel::getPassword() const {
	return this->_password;
}

bool Channel::isChannelInviteOnly() const {
	return this->_isInviteOnly;
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

void Channel::setModerators(std::vector<Client *> moderators) {
	this->_moderators = moderators;
}

void Channel::setMessages(std::vector<std::string> messages)
{
	this->_messages = messages;
}

void Channel::setPassword(std::string password) {
	this->_password = password;
}

void Channel::setIsChannelInviteOnly(bool isInviteOnly) {
	this->_isInviteOnly = isInviteOnly;
}

void Channel::addClient(Client *client) // What if the client is already in the channel
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

void Channel::addModerator(Client *moderator) { // Same as addClient
	this->_moderators.push_back(moderator);
}

void Channel::removeModerator(Client *moderator) {
	for (std::vector<Client *>::iterator it = this->_moderators.begin(); it != this->_moderators.end() ;it++) {
		if (*it == moderator) {
			this->_moderators.erase(it);
			return ;
		}
	}
}

void Channel::addMessage(std::string message)
{
	message = "Server: " + message;
	this->_messages.push_back(message);
	this->_sendToAll(message);
}

void Channel::addMessage(std::string message, Client *client)
{
	message = client->getNickname() + ": " + message;
	this->_messages.push_back(message);
	this->_sendToAllButOne(message, client);
}

void Channel::_sendToAll(std::string message)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		(*it)->send(message);
		it++;
	}
}

void Channel::_sendToAllButOne(std::string message, Client *client)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (*it != client)
			(*it)->send(message);
		it++;
	}
}
