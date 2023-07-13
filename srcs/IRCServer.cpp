/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:19:33 by yoel              #+#    #+#             */
/*   Updated: 2023/07/14 17:27:57 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"

IRCServer::IRCServer()
{
	;
}

IRCServer::~IRCServer()
{
	;
}

IRCServer::IRCServer(const IRCServer &other)
{
	*this = other;
}

IRCServer &IRCServer::operator=(const IRCServer &other)
{
	(void)other;
	return (*this);
}

int IRCServer::init(int port, std::string const & password, std::string const & motd)
{
	this->_port = port;
	this->_password = password;
	this->_motd = motd;
	this->_initSocket();
	this->_initFdSets();
	return 0;
}

int IRCServer::run()
{
	while (1)
	{
		this->_select();
		this->_accept();
		for (std::map<int, Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		{
			this->_recv(*it->second);
			this->_send(*it->second);
		}
	}
}

int IRCServer::_initSocket()
{
	std::cout << "Openning socket... ";
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	if (fcntl(this->_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	std::cout << "Binding socket... ";
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(this->_port);
	if (bind(this->_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Listening socket... ";
	if (listen(this->_socket, 32) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;
	return 0;
}

void IRCServer::_initFdSets()
{
	FD_ZERO(&this->_allfds);
	FD_ZERO(&this->_readfds);
	FD_ZERO(&this->_writefds);
	FD_SET(this->_socket, &this->_allfds);
	this->_maxfd = this->_socket;
}

int IRCServer::_select()
{
	this->_readfds = this->_allfds;
	this->_writefds = this->_allfds;
	if (select(this->_maxfd + 1, &this->_readfds, &this->_writefds, NULL, NULL) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	return 0;
}

int IRCServer::_accept()
{
	if (FD_ISSET(this->_socket, &this->_readfds))
	{
		int client = accept(this->_socket, NULL, NULL);
		if (client < 0)
		{
			std::cout << "Error" << std::endl;
			return 1;
		}
		else
		{
			std::cout << "New client connected" << std::endl;
			this->_addClient(client);
		}
		FD_SET(client, &this->_allfds);
		if (client > this->_maxfd)
			this->_maxfd = client;
	}
	return 0;
}

int IRCServer::_recv(Client & client)
{
	if (FD_ISSET(client.getSocket(), &this->_readfds))
	{
		char buffer[1024];
		bzero(buffer, 1024);
		int ret = recv(client.getSocket(), buffer, 1023, 0);
		if (ret < 0)
		{
			std::cout << "Error" << std::endl;
			return 1;
		}
		else if (ret == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			this->_removeClient(client);
		}
		else
		{
			buffer[ret] = '\0';
			std::cout << "Received: " << buffer << std::endl;
			client.setRequest(buffer);
		}
	}
	return 0;
}

void IRCServer::_addClient(int fd)
{
	this->_clients.insert(std::pair<int, Client *>(fd, new Client(fd)));
}

void IRCServer::_removeClient(Client & client)
{
	close(client.getSocket());
	FD_CLR(client.getSocket(), &this->_allfds);
	this->_clients.erase(client.getSocket());
}

int IRCServer::_send(Client & client)
{
	if (FD_ISSET(client.getSocket(), &this->_writefds))
	{
		std::string buffer = client.getResponse();
		if (buffer.size() > 0)
		{
			int ret = send(client.getSocket(), buffer.c_str(), buffer.size(), 0);
			if (ret < 0)
			{
				std::cout << "Error" << std::endl;
				return 1;
			}
			else
			{
				std::cout << "Sent: " << buffer << std::endl;
				client.setResponse("");
			}
		}
	}
	return 0;
}

int sendToClient(Client & client, std::string const & message)
{
	client.setResponse(message);
	return 0;
}
