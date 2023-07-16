/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:55:38 by yoel              #+#    #+#             */
/*   Updated: 2023/07/16 19:44:42 by ycornamu         ###   ########.fr       */
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

Message::Message(std::string message) : _message(message), _prefix(""), _params(0)
{
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

	this->_message = message;
	if (message != "")
		this->_parse();
}

void Message::_parse()
{
	std::string param;
	std::stringstream ss(this->_message);

	std::getline(ss, this->_prefix, ' ');
	this->_prefix.erase(std::remove(this->_prefix.begin(), this->_prefix.end(), '\r'), this->_prefix.end());
	while (std::getline(ss, param, ' '))
	{
		param.erase(std::remove(param.begin(), param.end(), '\r'), param.end());
		this->_params.push_back(param);
	}
}
