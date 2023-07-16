/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:55:38 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 22:50:27 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Message.hpp"
#include <sstream>

// class Message
// {
// 	private:
// 		std::string _message;
// 		std::string _prefix;
// 		std::vector<std::string> _params;
// 	
// 	public:
// 		Message();
// 		Message(std::string message);
// 		Message(Message const & src);
// 		~Message();
// 		Message & operator=(Message const & src);
// 	
// 		std::string getMessage() const;
// 		std::string getPrefix() const;
// 		std::vector<std::string> getParams() const;
// 	
// 		void setMessage(std::string message);
// 	
// 		void parseMessage();
// };

Message::Message() : _message(""), _prefix(""), _params(0)
{
	;
}

Message::Message(std::string message) : _prefix(""), _params(0)
{
	message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
	this->_message = message;
	if (message != "")
		this->_parse();
}

Message::Message(Message const & src)
{
	*this = src;
}

Message::~Message()
{
	;
}

Message & Message::operator=(Message const & src)
{
	if (this != &src)
	{
		this->_message = src._message;
		this->_prefix = src._prefix;
		this->_params = src._params;
	}
	return (*this);
}

std::string Message::getMessage() const
{
	return (this->_message);
}

std::string Message::getPrefix() const
{
	return (this->_prefix);
}

std::vector<std::string> Message::getParams() const
{
	return (this->_params);
}

void Message::setMessage(std::string message)
{

	message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
	this->_message = message;
	if (message != "")
		this->_parse();
}

void Message::_parse()
{
	std::string param;
	std::stringstream ss(this->_message);

	if (this->_message[0] == ':')
		std::getline(ss, this->_sender, ' ');

	std::getline(ss, this->_prefix, ' ');

	while (std::getline(ss, param, ' '))
	{
		if (param[0] == ':')
		{
			std::string tmp;
			
			while (std::getline(ss, tmp, ' '))
				param += " " + tmp;
			this->_params.push_back(param);
			break ;
		}
		this->_params.push_back(param);
	}
}
