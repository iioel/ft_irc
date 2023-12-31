/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 13:19:33 by yoel              #+#    #+#             */
/*   Updated: 2023/07/30 18:34:06 by lulutalu         ###   ########.fr       */
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
	this->_creation_time = time(NULL);
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
		for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
		{
			if (this->_recv(**it))
				continue;
			this->_send(**it);
		}

		for (size_t i = 0; i < this->_clients.size(); ++i)
		{
			if ((this->_clients[i]->isRemoved() && ! this->_clients[i]->getResponse().size()) || this->_clients[i]->ping())
				this->_removeClient(this->_clients[i--]);
		}
	}
}

int IRCServer::_initSocket()
{
	std::cout << "Openning socket... ";
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
	{
		std::cout << "Error opening socket..." << std::endl;
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
		std::cout << "Error binding socket..." << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Listening socket... ";
	if (listen(this->_socket, 32) < 0)
	{
		std::cout << "Error listening socket..." << std::endl;
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
		struct sockaddr addr;
		socklen_t addrlen = sizeof(addr);
		int client = accept(this->_socket, &addr, &addrlen);
		if (client < 0)
		{
			std::cout << "Error accepting connection..." << std::endl;
			return 1;
		}
		else
		{
			std::cout << "New client connected" << std::endl;
			std::string addrstr = inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr);
			this->_addClient(client, addrstr);
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
			std::cout << "Error reciving..." << std::endl;
			return 1;
		}
		else if (ret == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			client.remove();
			return 1;
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
				std::cout << "Error sending..." << std::endl;
				return 1;
			}
			else if (ret < static_cast<int>(buffer.size()))
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
		else if (request.getPrefix() == "QUIT")
			this->_processQuit(request, client);
		else if (request.getPrefix() == "JOIN")
			this->_processJoin(request, client);
		else if (request.getPrefix() == "PRIVMSG")
			this->_processPrivmsg(request, client);
		else if (request.getPrefix() == "KICK")
			this->_processKick(request, client);
		else if (request.getPrefix() == "NOTICE")
			this->_processNotice(request, client);
		else if (request.getPrefix() == "NAMES")
			this->_processNames(request, client);
		else if (request.getPrefix() == "LIST")
			this->_processList(request, client);
		else if (request.getPrefix() == "INVITE")
			this->_processInvite(request, client);
		else if (request.getPrefix() == "TOPIC")
			this->_processTopic(request, client);
		else if (request.getPrefix() == "PART")
			this->_processPart(request, client);
		else if (request.getPrefix() == "MODE")
			this->_processMode(request, client);
		else if (request.getPrefix() == "WHO")
			this->_processWho(request, client);
		else if (request.getPrefix() == "WHOIS")
			this->_processWhois(request, client);
		else
		{
			client.send(":" + this->_server_name + " " + ERR_UNKNOWNCOMMAND + " "
				+ client.getNickname() + " " + request.getPrefix()
				+ " :Unknown command");
		}
	}
}

void IRCServer::_addClient(int fd, std::string addr)
{
	this->_clients.push_back(new Client(fd, this->_allwritefds, addr));
}

void IRCServer::_removeClient(Client * client)
{
	std::cout << "Closing socket" << std::endl;
	close(client->getSocket());
	FD_CLR(client->getSocket(), &this->_allfds);
	FD_CLR(client->getSocket(), &this->_readfds);
	FD_CLR(client->getSocket(), &this->_writefds);
	FD_CLR(client->getSocket(), &this->_allwritefds);
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		(*it)->removeMember(client);
	for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (**it == *client)
		{
			delete *it;
			this->_clients.erase(it);
			break;
		}
	}
}

void IRCServer::_removeChannel(Channel * channel)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		if (**it == *channel)
		{
			delete *it;
			this->_channels.erase(it);
			break;
		}
	}

}

Channel *IRCServer::checkChannelExist(std::string channelName)
{
	for (std::vector<Channel *>::iterator it = this->_channels.begin(); 
			it != this->_channels.end(); it++)
	{
		if ((*it)->getName() == channelName)
			return (*it);
	}
	return (NULL);
}

Client	*IRCServer::findClientByNickname(std::string nickname) {
	for (std::vector<Client *>::iterator it = this->_clients.begin();
			it != this->_clients.end(); it++) {
		if ((*it)->getNickname() == nickname) {
			return (*it);
		}
	}
	return (NULL);
}

void IRCServer::_sendToAll(std::string message)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		(*it)->send(message);
		it++;
	}
}

void IRCServer::_sendToAllButOne(std::string message, Client & client)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if ((*it)->getSocket() != client.getSocket())
			(*it)->send(message);
		it++;
	}
}

void IRCServer::_sendWelcome(Client & client)
{
	client.send(":" + this->_server_name + " " + RPL_WELCOME + " "
			+ client.getNickname() + " :Welcome to the Internet Relay Network "
			+ client.getFQUN());
	client.send(":" + this->_server_name + " " + RPL_YOURHOST + " "
			+ client.getNickname() + " :Your host is " + this->_server_name
			+ ", running version 1.0");
	client.send(":" + this->_server_name + " " + RPL_CREATED + " "
			+ client.getNickname() + " :This server was created "
			+ ctime(&this->_creation_time));
	//client.send(":" + this->_server_name + " " + RPL_MYINFO + " "
	//		+ client.getNickname() + " 1.0  i itkol");
	this->_sendMOTD(client);
}

void IRCServer::_sendMOTD(Client & client)
{
	client.send(":" + this->_server_name + " " + RPL_MOTDSTART + " "
			+ client.getFQUN()
			+ " :-                  Message of the day                   -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-                                                       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :- Welcome to our ft_irc server made @42Lausanne !       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-                                                       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-         :::      ::::::::                             -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-       :+:      :+:    :+:                             -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-     +:+ +:+         +:+                               -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-   +#+  +:+       +#+                                  -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :- +#+#+#+#+#+   +#+                                     -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-      #+#    #+#                                       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-     ###   ########lausanne.ch                         -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-                                                       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :- Enjoy using this server !                             -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-                                                       -");
	client.send(":" + this->_server_name + " " + RPL_MOTD + " " + client.getFQUN()
			+ " :-                       Your hosts: lduboulo & ycornamu -");
	client.send(":" + this->_server_name + " " + RPL_ENDOFMOTD + " "
			+ client.getFQUN() + " :End of MOTD command");
}
