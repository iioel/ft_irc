/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:19:33 by yoel              #+#    #+#             */
/*   Updated: 2023/07/18 16:44:59 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "IRCServer.hpp"
#include "Message.hpp"
#include "Reply.hpp"

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

int IRCServer::init(int port, std::string const & password)
{
	this->_port = port;
	this->_password = password;
	this->_server_name = "server.42.fr";
	this->_motd = "Welcome to the IRC server";
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
		for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		{
			if (this->_recv(*it))
				break;
			this->_send(*it);
			if (it->ping() || (it->isRemoved() && it->getResponse().size() == 0))
			{
				this->_removeClient(*it);
				break;
			}

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
	FD_ZERO(&this->_allwritefds);
	FD_ZERO(&this->_writefds);
	FD_SET(this->_socket, &this->_allfds);
	this->_maxfd = this->_socket;
}

int IRCServer::_select()
{
	this->_readfds = this->_allfds;
	this->_writefds = this->_allwritefds;
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
			return (1);
		}
		else
		{
			buffer[ret] = '\0';
			client.addRequest(buffer);
			this->_processRequest(client);
		}
	}
	return 0;
}

void IRCServer::_processRequest(Client & client)
{
	std::string line;

	while ((line = client.getRequest()).size() != 0 && ! client.isRemoved())
	{
		Message request = Message(line);
		std::cout << "Received message <== : " << request.getMessage() << std::endl;

		if (request.getPrefix() == "CAP")
			this->_processCap(request, client);
		else if (request.getPrefix() == "PONG")
			this->_processPong(request, client);
		else if (request.getPrefix() == "PASS")
			this->_processPass(request, client);
		else if (request.getPrefix() == "NICK")
			this->_processNick(request, client);
		else if (request.getPrefix() == "USER")
			this->_processUser(request, client);
		else if (!client.isRegistered())
		{
			client.send(":" + this->_server_name + " " + ERR_NOTREGISTERED + " "
					+ client.getNickname() + " :You have not registered");
			break;
		}
		else if (request.getPrefix() == "PING")//				ycornamu
			this->_processPing(request, client);
//		else if (request.getPrefix() == "OPER")
//			this->_processOper(request, client);
		else if (request.getPrefix() == "QUIT")
			this->_processQuit(request, client);
//	------------------------------------------------------------------------------
//		else if (request.getPrefix() == "JOIN")
//			this->_processJoin(request, client);
//		else if (request.getPrefix() == "PRIVMSG")
//			this->_processPrivmsg(request, client);
//		else if (request.getPrefix() == "MOTD")
//			this->_processMotd(request, client);
//		else if (request.getPrefix() == "KICK")
//			this->_processKick(request, client);
//		else if (request.getPrefix() == "INVITE")			lduboulo
//			this->_processInvite(request, client);
//		else if (request.getPrefix() == "TOPIC")
//			this->_processTopic(request, client);
//		else if (request.getPrefix() == "MODE")
//			this->_processMode(request, client);
	}
}

void IRCServer::_addClient(int fd)
{
	this->_clients.push_back(Client(fd, this->_allwritefds));
}

void IRCServer::_removeClient(Client & client)
{
	std::cout << "Closing socket" << std::endl;
	close(client.getSocket());
	FD_CLR(client.getSocket(), &this->_allfds);
	FD_CLR(client.getSocket(), &this->_readfds);
	FD_CLR(client.getSocket(), &this->_writefds);
	FD_CLR(client.getSocket(), &this->_allwritefds);
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getSocket() == client.getSocket())
		{
			this->_clients.erase(it);
			break;
		}
	}
}

int IRCServer::_send(Client & client)
{
	if (FD_ISSET(client.getSocket(), &this->_writefds))
	{
		std::cout << "Sending" << std::endl;
		std::string buffer = client.getResponse();
		if (buffer.size() > 0)
		{
			size_t ret = send(client.getSocket(), buffer.c_str(), buffer.size(), 0);
			if (ret < 0)
			{
				std::cout << "Error" << std::endl;
				return 1;
			}
			else if (ret < buffer.size())
				client.setResponse(buffer.substr(ret));
			else
			{
				client.clearResponse();
				FD_CLR(client.getSocket(), &this->_allwritefds);
			}
		}
	}
	return 0;
}

void IRCServer::_sendToAll(std::string message)
{
	std::vector<Client>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		it->send(message);
		it++;
	}
}

void IRCServer::_sendToAllButOne(std::string message, Client & client)
{
	std::vector<Client>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (it->getSocket() != client.getSocket())
			it->send(message);
		it++;
	}
}
