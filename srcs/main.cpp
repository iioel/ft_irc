/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:35:17 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/14 02:35:38 by yoel             ###   ########.fr       */
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

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

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

std::string handle_request(std::string request, int fd)
{
	std::string response = "";
	std::string line;
	std::stringstream ss(request);

	while (std::getline(ss, line))
	{
		if (line.find("CAP LS") != std::string::npos)
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
		if (line.find("MODE") != std::string::npos)
		{
			response += "MODE " + line.substr(5) + "\r\n";
			continue ;
		}
		if (line.find("QUIT") != std::string::npos)
		{
			response += "QUIT " + line.substr(5) + "\r\n";
			std::cout << "Closing connection... ";
			if (close(fd) < 0)
			{
				std::cout << "Error" << std::endl;
				return "ERROR";
			}
			continue ;
		}
		continue ;
	}
	return response;
}

int main(int argc, char const *argv[])
{
	(void)argc;
	(void)argv;

	fd_set originfds, readfds;
	int server_fd = create_socket();
	if (server_fd == 1)
		return 1;
	int max_fd = server_fd;
	char buffer[BUFFER_SIZE];

	FD_ZERO(&originfds);
	FD_SET(server_fd, &originfds);

	while(1)
	{
		readfds = originfds;

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
			for (int i = 0; i < max_fd + 1; i++)
			{
				if (fd_is_readable(i, &readfds))
				{
					if (i == server_fd)
					{
						std::cout << "New connection... ";
						int new_socket_fd = accept(server_fd, NULL, NULL);
						if (new_socket_fd < 0)
						{
							std::cout << "Error" << std::endl;
							return 1;
						}
						std::cout << "fd: " << new_socket_fd << " OK" << std::endl;

						std::cout << "OK" << std::endl;
						FD_SET(new_socket_fd, &originfds);
						if (new_socket_fd > max_fd)
							max_fd = new_socket_fd;
						std::cout << "New max_fd: " << max_fd << std::endl;
					}
					else
					{
						//std::cout << "Reading... ";
						int valread = read(i, buffer, BUFFER_SIZE);
						if (valread < 0)
						{
							std::cout << "Error" << std::endl;
							return 1;
						}
						else if (valread == 0)
						{
							std::cout << "Closing connection... ";
							if (close(i) < 0)
							{
								std::cout << "Error" << std::endl;
								return 1;
							}
							std::cout << "fd: " << i << " OK" << std::endl;
							FD_CLR(i, &originfds);
							if (close(i) < 0)
							{
								std::cout << "Error" << std::endl;
								return 1;
							}
							if (i == max_fd)
							{
								while (max_fd > 0 && !fd_is_readable(max_fd, &originfds))
									max_fd--;
								std::cout << "New max_fd: " << max_fd << std::endl;
							}
						}
						else
						{
							std::cout << "OK" << std::endl;
							//std::cout << "Handling request... ";
							std::string request(buffer);
							std::cout << "request: " << request << " ";
							std::string response = handle_request(request, i);
							if (response == "ERROR")
							{
								std::cout << "ttError" << std::endl;
								return 1;
							}
							//std::cout << "OK" << std::endl;
							//std::cout << "Sending response... ";
							std::cout << "response: " << response << " ";
							if (send(i, response.c_str(), response.length(), 0) < 0)
							{
								std::cout << "Error" << std::endl;
								return 1;
							}
							//std::cout << "OK" << std::endl;
						}
					}
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
