/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:00:14 by yoel              #+#    #+#             */
/*   Updated: 2023/07/17 20:57:12 by ycornamu         ###   ########.fr       */
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
# include "Message.hpp"

class IRCServer
{
	public:
		IRCServer();
		IRCServer(IRCServer const & src);
		~IRCServer();
		IRCServer & operator=(IRCServer const & src);

		int init(int port, std::string const & password);
		int run();

		int sendToClient(std::string const & message, Client & client);

	private:
		int 	_socket;
		int 	_port;
		int 	_maxfd;
		fd_set 	_allfds;
		fd_set 	_readfds;
		fd_set 	_allwritefds;
		fd_set 	_writefds;
		std::string 				_server_name;
		std::string 				_motd;
		std::string 				_password;
		std::vector<Client> 		_clients;
		std::vector<Channel> 		_channels;

		int 	_initSocket();
		void 	_initFdSets();

		int			_select();
		int			_accept();
		int			_recv(Client & client);
		int			_send(Client & client);
		void		_addClient(int fd);
		void		_removeClient(Client & client);
		void		_processRequest(Client & client);
		void		_sendToAll(std::string message);
		void 		_sendToAllButOne(std::string message, Client & client);


		int	_processCap(Message & request, Client & client);
//		int	_processPass(Message & request, Client & client);
		int	_processNick(Message & request, Client & client);
		int	_processUser(Message & request, Client & client);
		int	_processPing(Message & request, Client & client);
		int	_processPong(Message & request, Client & client);
//		int	_processOper(Message & request, Client & client);
//		int	_processQuit(Message & request, Client & client);
		int	_processJoin(Message & request, Client & client);
//		int	_processPrivmsg(Message & request, Client & client);
//		int	_processMotd(Message & request, Client & client);
//		int	_processKick(Message & request, Client & client);
//		int	_processInvite(Message & request, Client & client);
//		int	_processTopic(Message & request, Client & client);
//		int	_processMode(Message & request, Client & client);
};

#endif
