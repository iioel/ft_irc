/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:35:17 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/13 17:56:19 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>

#define PORT 6667

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;
	std::cout << "Openning socket... ";
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Binding socket... ";
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Listening socket... ";
	if (listen(socket_fd, 10) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Accepting connection... ";
	int client_fd = accept(socket_fd, NULL, NULL);
	if (client_fd < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	bool nick = false;
	bool user = false;
	bool welcome = false;
	bool cap = false;
	while (1)
	{
		std::string RPL = "";
		std::cout << "Reading request... ";
		char buffer[1024];
		int read_size = read(client_fd, buffer, 1024);
		if (read_size < 0)
		{
			std::cout << "Error" << std::endl;
			return 1;
		}
		std::cout << "OK" << std::endl;

		if (read_size == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			return 0;
		}
		else if (read_size < 1024)
			buffer[read_size] = '\0';
		std::cout << "Request: " << buffer << std::endl;
		std::string request(buffer);
		std::stringstream iss(request);
		std::string req;
		while(std::getline(iss, req, '\n'))
		{
			std::cout << "Req: " << req << std::endl;
			if (req.find("CAP LS 302") != std::string::npos)
			{
				RPL = "CAP * LS :multi-prefix\r\n";
				cap = true;
			}
			else if (req.find("CAP REQ :multi-prefix") != std::string::npos)
				RPL = "CAP * ACK :multi-prefix\r\n";
			else if	(req.find("CAP END") != std::string::npos)
				cap = false;
			if (cap)
				break ;
			else if (req.find("NICK") != std::string::npos)
			{
				nick = true;
				RPL += ":root!root@servername NICK root\r\n";
			}
			else if (req.find("USER") != std::string::npos)
			{
				user = true;
				//RPL += "USER root\r\n";
			}
			else if (req.find("PING") != std::string::npos)
				RPL += "PONG servername\r\n";
			else if (req.find("MODE") != std::string::npos)
				RPL += ":servername 221 root +i\r\n";
			else if (req.find("JOIN") != std::string::npos)
				RPL += ":servername 332 root #chan :Welcome to #chan\r\n";
		}
		std::cout << "nick: " << nick << std::endl;
		std::cout << "user: " << user << std::endl;
		std::cout << "welcome: " << welcome << std::endl;

		if (nick && user && !welcome)
		{
			RPL += ":servername 001 root Welcome to MY awesome serv ! root!root@servername\r\n\
:servername 002 root Your host is servername, running version 1.0\r\n\
:servername 003 root This server was created sometime\r\n\
:servername 004 root servername 1.0 aiwroOs biklmnopstvr\r\n";
			welcome = true;
		}

		std::cout << "RPL: " << RPL << std::endl;
		if (RPL != "")
		{
			std::cout << "Sending response... ";
			//char response[] = RPL.c_str();
			if (write(client_fd, RPL.c_str(), strlen(RPL.c_str())) < 0)
			{
				std::cout << "Error" << std::endl;
				return 1;
			}
			std::cout << "OK" << std::endl;
		}
	
	}

	std::cout << "Closing connection... ";
	if (close(client_fd) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Closing socket... ";
	if (close(socket_fd) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;
	return 0;
}
