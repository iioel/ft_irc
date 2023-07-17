/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:12:41 by yoel              #+#    #+#             */
/*   Updated: 2023/07/17 18:35:23 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <sstream>
# include <sys/time.h>
# include <sys/select.h>

class Client
{
	private:
		Client();

		int				_socket;
		bool			_isConnected;
		bool			_isRegistered;
		bool			_isPong;
		struct timeval	_pingTime;
		std::string		_pingStr;
		std::string		_nickname;
		std::string		_username;
		std::string		_realname;
		std::string		_request;
		std::string		_response;

	public:
		Client(int fd);
		Client(Client const & src);
		~Client();
		Client & operator=(Client const & src);

		bool isRequest(fd_set *readfds);
		bool isResponse(fd_set *writefds);

		// Getter
		int			getSocket() const;
		bool		isConnected() const;
		bool		isRegistered() const;
		bool		isPong() const;
		std::string	getPingStr() const;
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string	getRequest();
		std::string	getResponse() const;
		struct timeval	getPingTime() const;

		// Setter
		void	setSocket(int fd);
		void	setConnected(bool isConnected);
		void	setRegistered(bool isRegistered);
		void	setPong(bool isPong);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);
		void	addRequest(std::string request);
		void	clearResponse();

		// Other

		int		send(std::string msg);
		int 	ping();
};

#endif
