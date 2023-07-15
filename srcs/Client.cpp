/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:53:25 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 14:53:47 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/select.h>

#include "Client.hpp"

Client::Client(int fd)
{
	this->_socket = fd;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_request = "";
	this->_response = "";
}

Client::Client(Client const & src)
{
	*this = src;
}

Client::~Client()
{
	;
}

Client & Client::operator=(Client const & src)
{
	if (this != &src)
	{
		this->_socket = src._socket;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_realname = src._realname;
		this->_request = src._request;
		this->_response = src._response;
	}
	return (*this);
}

bool Client::isRequest(fd_set *readfds)
{
	if (FD_ISSET(this->_socket, readfds))
		return (true);
	return (false);
}

bool Client::isResponse(fd_set *writefds)
{
	if (FD_ISSET(this->_socket, writefds))
		return (true);
	return (false);
}

// Getter
int			Client::getSocket() const
{
	return (this->_socket);
}

std::string	Client::getNickname() const
{
	return (this->_nickname);
}

std::string	Client::getUsername() const
{
	return (this->_username);
}

std::string	Client::getRealname() const
{
	return (this->_realname);
}

std::string	Client::getRequest() const
{
	return (this->_request);
}

std::string	Client::getResponse() const
{
	return (this->_response);
}

// Setter

void	Client::setSocket(int fd)
{
	this->_socket = fd;
}

void	Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::setUsername(std::string username)
{
	this->_username = username;
}

void	Client::setRealname(std::string realname)
{
	this->_realname = realname;
}

void	Client::setRequest(std::string request)
{
	this->_request = request;
}

void	Client::addResponse(std::string response)
{
	this->_response += response;
}

void	Client::clearResponse()
{
	this->_response.clear();
}

bool Client::isRegistered() const
{
	return (this->_isRegistered);
}

void Client::setRegistered(bool isRegistered)
{
	this->_isRegistered = isRegistered;
}

bool Client::isConnected() const
{
	return (this->_isConnected);
}

void Client::setConnected(bool isConnected)
{
	this->_isConnected = isConnected;
}
