/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:35:17 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/14 00:57:18 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <sys/select.h>
#include <cerrno>
#include <errno.h>

#define PORT 6667
#define MAX_FD 30
#define BUFFER_SIZE 1024

int create_socket()
{
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
	return socket_fd;
}

bool fd_is_readable(int fd, fd_set *readfds)
{
	return fd > 0 && FD_ISSET(fd, readfds);
}

bool fd_is_writable(int fd, fd_set *writefds)
{
	return fd > 0 && FD_ISSET(fd, writefds);
}

std::string handle_request(std::string request, int clients_fds[MAX_FD], int index)
{
	std::string response;
	std::string line;
	std::stringstream ss(request);

	while (std::getline(ss, line))
	{
		if (line.find("CAP LS 302") != std::string::npos)
		{
			response += "CAP * LS :\r\n";
			continue ;
		}
		if (line.find("NICK") != std::string::npos)
		{
			response += "NICK " + line.substr(5) + "\r\n";
			continue ;
		}
		if (line.find("PING") != std::string::npos)
		{
			response += "PONG " + line.substr(5) + "\r\n";
			continue ;
		}
		if (line.find("USER") != std::string::npos)
		{
			response += ":localhost 001 " + line.substr(5) + " :Welcome to the Internet Relay Network " + line.substr(5) + "\r\n";
			continue ;
		}
		if (line.find("JOIN") != std::string::npos)
		{
			response += "JOIN " + line.substr(5) + "\r\n";
			continue ;
		}
		if (line.find("PRIVMSG") != std::string::npos)
		{
			response += "PRIVMSG " + line.substr(8) + "\r\n";
			continue ;
		}
		if (line.find("QUIT") != std::string::npos)
		{
			response += "QUIT " + line.substr(5) + "\r\n";
			std::cout << "Closing connection... ";
			if (close(clients_fds[index]) < 0)
			{
				std::cout << "Error" << std::endl;
				return "";
			}
			std::cout << "fd: " << clients_fds[index] << " OK" << std::endl;
			clients_fds[index] = -1;
			continue ;
		}
	}
	return response;
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	fd_set originfds;
	fd_set readfds;
	fd_set writefds;
	int max_fd = MAX_FD;
	int server_fd = create_socket();
	if (server_fd == 1)
		return 1;
	int clients_fds[MAX_FD];
	int new_socket_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	char buffer[BUFFER_SIZE];

	for (int i = 0; i < 30; i++)
		clients_fds[i] = -1;
	clients_fds[0] = server_fd;
	max_fd = server_fd;
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	FD_ZERO(&originfds);
	FD_SET(server_fd, &originfds);

	while(1)
	{
		readfds = originfds;
		writefds = originfds;

		std::cout << "Waiting for activity... ";
		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		std::cout << "activity: " << activity << " ";

		if (activity < 0)
		{
			std::cout << "Error" << std::endl;
			return 1;
		}
		else if (activity == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue ;
		}

		if (activity > 0)
		{
			std::cout << "OK" << std::endl;
			if (FD_ISSET(server_fd, &readfds))
			{
				std::cout << "New connection... ";
				new_socket_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
				if (new_socket_fd < 0)
				{
					std::cout << "Error" << std::endl;
					return 1;
				}
				std::cout << "fd: " << new_socket_fd << " OK" << std::endl;

				std::cout << "Adding to list... ";
				std::cout << "Max fd: " << max_fd << " ";
				for (int i = 0; i < 30; i++)
				{
					if (clients_fds[i] == -1)
					{
						fcntl(new_socket_fd, F_SETFL, O_NONBLOCK);
						clients_fds[i] = new_socket_fd;
						if (new_socket_fd > max_fd)
							max_fd = new_socket_fd + 2;
						std::cout << "id: " << i << " OK" << std::endl;
						i = 30;
					}
				}
				continue ;
			}

			for (int i=1; i < MAX_FD; i++)
			{
				if (FD_ISSET(i, &readfds))
				{
					std::cout << "Checking fd: " << i << "... ";
					std::cout << "Reading request... ";
					std::string request;
					int read_size = 0;
					do
					{
						bzero(buffer, BUFFER_SIZE);
						std::cout << "Reading... ";
						read_size = recv(i, buffer, BUFFER_SIZE-1, 0);
						//read_size = read(new_socket_fd, buffer, BUFFER_SIZE-1);
						if (read_size < 0)
						{
							std::cout << "Error" << std::endl;
							std::cout << errno << std::endl;
							std::cout << strerror(errno);
							return 1;
						}
						if (read_size == 0)
						{
							std::cout << "Client disconnected" << std::endl;
							close(i);
							clients_fds[i] = -1;
							continue ;
						}
						request += buffer;
					} while (read_size == BUFFER_SIZE);
					std::cout << "OK" << std::endl;
					std::cout << "Request: " << request << std::endl;

					std::string response = handle_request(request, clients_fds, i);
					std::cout << "Sending response... ";
					if (send(i, response.c_str(), response.length(), 0) < 0)
					{
						std::cout << "Error" << std::endl;
						return 1;
					}
					std::cout << "OK" << std::endl;
					continue ;
				}
			}
		}
	}

/*	bool nick = false;
//	bool user = false;
//	bool welcome = false;
//	bool cap = false;
//	while (1)
//	{
//		std::string RPL = "";
//		std::cout << "Reading request... ";
//		char buffer[1024];
//		int read_size = read(client_fd, buffer, 1024);
//		if (read_size < 0)
//		{
//			std::cout << "Error" << std::endl;
//			return 1;
//		}
//		std::cout << "OK" << std::endl;
//
//		if (read_size == 0)
//		{
//			std::cout << "Client disconnected" << std::endl;
//			return 0;
//		}
//		else if (read_size < 1024)
//			buffer[read_size] = '\0';
//		std::cout << "Request: " << buffer << std::endl;
//		std::string request(buffer);
//		std::stringstream iss(request);
//		std::string req;
//		while(std::getline(iss, req, '\n'))
//		{
//			std::cout << "Req: " << req << std::endl;
//			if (req.find("CAP LS 302") != std::string::npos)
//			{
//				RPL = "CAP * LS :multi-prefix\r\n";
//				cap = true;
//			}
//			else if (req.find("CAP REQ :multi-prefix") != std::string::npos)
//				RPL = "CAP * ACK :multi-prefix\r\n";
//			else if	(req.find("CAP END") != std::string::npos)
//				cap = false;
//			if (cap)
//				break ;
//			else if (req.find("NICK") != std::string::npos)
//			{
//				nick = true;
//				RPL += ":root!root@servername NICK root\r\n";
//			}
//			else if (req.find("USER") != std::string::npos)
//			{
//				user = true;
//				//RPL += "USER root\r\n";
//			}
//			else if (req.find("PING") != std::string::npos)
//				RPL += "PONG servername\r\n";
//			else if (req.find("MODE") != std::string::npos)
//				RPL += ":servername 221 root +i\r\n";
//			else if (req.find("JOIN") != std::string::npos)
//				RPL += ":servername 332 root #chan :Welcome to #chan\r\n";
//		}
//		std::cout << "nick: " << nick << std::endl;
//		std::cout << "user: " << user << std::endl;
//		std::cout << "welcome: " << welcome << std::endl;
//
//		if (nick && user && !welcome)
//		{
//			RPL += ":servername 001 root Welcome to MY awesome serv ! root!root@servername\r\n\
//:servername 002 root Your host is servername, running version 1.0\r\n\
//:servername 003 root This server was created sometime\r\n\
//:servername 004 root servername 1.0 aiwroOs biklmnopstvr\r\n";
//			welcome = true;
//		}
//
//		std::cout << "RPL: " << RPL << std::endl;
//		if (RPL != "")
//		{
//			std::cout << "Sending response... ";
//			//char response[] = RPL.c_str();
//			if (write(client_fd, RPL.c_str(), strlen(RPL.c_str())) < 0)
//			{
//				std::cout << "Error" << std::endl;
//				return 1;
//			}
//			std::cout << "OK" << std::endl;
//		}
//	
//	}
//
//	std::cout << "Closing connection... ";
//	if (close(client_fd) < 0)
//	{
//		std::cout << "Error" << std::endl;
//		return 1;
//	}
*/	std::cout << "OK" << std::endl;

	std::cout << "Closing socket... ";
	if (close(server_fd) < 0)
	{
		std::cout << "Error" << std::endl;
		return 1;
	}
	std::cout << "OK" << std::endl;
	return 0;
}
