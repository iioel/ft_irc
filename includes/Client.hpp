/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:12:41 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 14:52:54 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <sys/select.h>

class Client
{
	private:
		Client();

		int			_socket;
		bool		_isConnected;
		bool		_isRegistered;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_request;
		std::string	_response;

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
		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string	getRequest() const;
		std::string	getResponse() const;

		// Setter
		void	setSocket(int fd);
		void	setConnected(bool isConnected);
		void	setRegistered(bool isRegistered);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);
		void	setRequest(std::string request);
		void	addResponse(std::string response);
		void	clearResponse();
};

#endif
