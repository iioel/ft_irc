/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 16:53:25 by yoel              #+#    #+#             */
/*   Updated: 2023/07/17 18:53:25 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/select.h>

#include "../includes/Client.hpp"

Client::Client(int fd)
{
	this->_socket = fd;
	this->_isConnected = false;
	this->_isRegistered = false;
	this->_isPong = true;
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_request = "";
	this->_response = "";
	gettimeofday(&this->_pingTime, NULL);
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
		this->_isConnected = src._isConnected;
		this->_isRegistered = src._isRegistered;
		this->_isPong = src._isPong;
		this->_pingTime = src._pingTime;
		this->_pingStr = src._pingStr;
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

std::string	Client::getRequest()
{
	size_t pos = this->_request.find("\r\n");
	if (pos != std::string::npos)
	{
		std::string request = this->_request.substr(0, pos);
		this->_request.erase(0, pos + 2);
		return (request);
	}
	return ("");
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

void	Client::addRequest(std::string request)
{
	this->_request += request;
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

void Client::setPong(bool isPong)
{
	this->_isPong = isPong;
}

bool Client::isPong() const
{
	return (this->_isPong);
}

std::string Client::getPingStr() const
{
	return (this->_pingStr);
}

struct timeval Client::getPingTime() const
{
	return (this->_pingTime);
}

// Other

int Client::send(std::string msg)
{
	std::cout << "Response message ==> : " << msg << std::endl;
	_response += msg + "\r\n";
	return (0);
}

int Client::ping()
{
	struct timeval now;
	gettimeofday(&now, NULL);

	if (now.tv_sec - this->_pingTime.tv_sec > 60 && ! this->_isPong)
	{
		this->send("ERROR :Closing Link: " + this->_nickname + " (Ping timeout)");
		return (1);
	}
	else if (now.tv_sec - this->_pingTime.tv_sec > 30 && this->_isPong)
	{
		std::ostringstream oss;

		this->_isPong = false;

		oss << std::rand() % 1000000;
		this->_pingStr = oss.str();

		this->send("PING :" + this->_pingStr);
		gettimeofday(&this->_pingTime, NULL);
		return (0);
	}
	return (0);
}


