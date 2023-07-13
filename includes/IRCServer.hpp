/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:00:14 by yoel              #+#    #+#             */
/*   Updated: 2023/07/14 17:26:53 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>

# include "perror.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class IRCServer
{
	public:
		IRCServer();
		IRCServer(IRCServer const & src);
		~IRCServer();
		IRCServer & operator=(IRCServer const & src);
		
		int init(int port, std::string const & password, std::string const & motd);
		int run();

		int sendToClient(std::string const & message, Client & client);

	private:
		int 	_socket;
		int 	_port;
		int 	_maxfd;
		fd_set 	_allfds;
		fd_set 	_readfds;
		fd_set 	_writefds;
		std::string 				_motd;
		std::string 				_password;
		std::map<int, Client *> 	_clients;
		std::vector<Channel> 		_channels;

		int 	_initSocket();
		void 	_initFdSets();

		int			_select();
		int			_accept();
		int			_recv(Client & client);
		int			_send(Client & client);
		void		_addClient(int fd);
		void		_removeClient(Client & client);
};

#endif
