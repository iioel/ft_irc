/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 17:00:52 by yoel              #+#    #+#             */
/*   Updated: 2023/07/22 16:45:21 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(std::string name) : _name(name)
{
	this->_creationTime = time(NULL);
	this->_password = "";
	this->_topic = "";
	this->_limit = 0;
	this->_inviteFlag = false;
	this->_topicFlag = false;
	this->_passwordFlag = false;
	this->_limitFlag = false;
}

Channel::Channel(std::string name, std::string password) : _name(name), _password(password)
{
	this->_creationTime = time(NULL);
	this->_topic = "";
	this->_limit = 0;
	this->_inviteFlag = false;
	this->_topicFlag = false;
	this->_passwordFlag = true;
	this->_limitFlag = false;
}

Channel::~Channel()
{
	;
}

Channel::Channel(Channel const & src)
{
	*this = src;
}

Channel & Channel::operator=(Channel const & src)
{
	if (this != &src)
	{
		this->_name = src._name;
		this->_password = src._password;
		this->_topic = src._topic;
		this->_limit = src._limit;
		this->_inviteFlag = src._inviteFlag;
		this->_topicFlag = src._topicFlag;
		this->_passwordFlag = src._passwordFlag;
		this->_limitFlag = src._limitFlag;
		this->_invited = src._invited;
		this->_members = src._members;
		this->_chanops = src._chanops;
		this->_chancreators = src._chancreators;
	}
	return (*this);
}

bool	Channel::operator==(Channel const & src) const
{
	return (this->_name == src._name);
}

// Getters

std::string Channel::getName() const
{
	return (this->_name);
}

std::string Channel::getPassword() const
{
	return (this->_password);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

std::string Channel::getStrCreationTime() const
{
	std::stringstream ss;
	ss << this->_creationTime;
	return (ss.str());
}

time_t Channel::getCreationTime() const
{
	return (this->_creationTime);
}

int Channel::getLimit() const
{
	return (this->_limit);
}

std::vector<Client *> Channel::getInvited() const
{
	return (this->_invited);
}

std::vector<Client *> Channel::getMembers() const
{
	return (this->_members);
}

std::vector<Client *> Channel::getChanops() const
{
	return (this->_chanops);
}

std::vector<Client *> Channel::getChancreators() const
{
	return (this->_chancreators);
}

bool Channel::isInvited(Client *client) const
{
	std::vector<Client *>::const_iterator it = this->_invited.begin();
	while (it != this->_invited.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isMember(Client *client) const
{
	std::vector<Client *>::const_iterator it = this->_members.begin();
	while (it != this->_members.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isChanop(Client *client) const
{
	std::vector<Client *>::const_iterator it = this->_chanops.begin();
	while (it != this->_chanops.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isChancreator(Client *client) const
{
	std::vector<Client *>::const_iterator it = this->_chancreators.begin();
	while (it != this->_chancreators.end())
	{
		if (*it == client)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::getInviteFlag() const
{
	return (this->_inviteFlag);
}

bool Channel::getTopicFlag() const
{
	return (this->_topicFlag);
}

bool Channel::getPasswordFlag() const
{
	return (this->_passwordFlag);
}

bool Channel::getLimitFlag() const
{
	return (this->_limitFlag);
}

Client	*Channel::getMemberFromName(std::string username) {
	for (std::vector<Client *>::iterator it = this->_members.begin();
			it != this->_members.end(); it++) {
		if (username == (*it)->getUsername()) {
			return (*it);
		}
	}
	return (NULL);
}

// Setters

void Channel::setName(std::string name)
{
	this->_name = name;
}

void Channel::setPassword(std::string password)
{
	this->_password = password;
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void Channel::setLimit(int limit)
{
	this->_limit = limit;
}

void Channel::setInvited(std::vector<Client *> invited)
{
	this->_invited = invited;
}

void Channel::setMembers(std::vector<Client *> members)
{
	this->_members = members;
}

void Channel::setChanops(std::vector<Client *> chanops)
{
	this->_chanops = chanops;
}

void Channel::setChancreators(std::vector<Client *> chancreators)
{
	this->_chancreators = chancreators;
}

void Channel::setInviteFlag(bool inviteFlag)
{
	this->_inviteFlag = inviteFlag;
}

void Channel::setTopicFlag(bool topicFlag)
{
	this->_topicFlag = topicFlag;
}

void Channel::setPasswordFlag(bool passwordFlag)
{
	this->_passwordFlag = passwordFlag;
}

void Channel::setLimitFlag(bool limitFlag)
{
	this->_limitFlag = limitFlag;
}

void Channel::addInvited(Client *client)
{
	if (!this->isInvited(client))
		this->_invited.push_back(client);
}

void Channel::removeInvited(Client *client)
{
	std::vector<Client *>::iterator it = this->_invited.begin();
	if (this->isInvited(client))
	{
		while (it != this->_invited.end())
		{
			if (*it == client)
			{
				this->_invited.erase(it);
				break ;
			}
			it++;
		}
	}
}

void Channel::addMember(Client *client)
{
	if (!this->isMember(client))
		this->_members.push_back(client);
}

void Channel::removeMember(Client *client)
{
	std::vector<Client *>::iterator it = this->_members.begin();
	if (this->isMember(client))
	{
		while (it != this->_members.end())
		{
			if (*it == client)
			{
				this->_members.erase(it);
				break ;
			}
			it++;
		}
	}
	this->removeInvited(client);
	this->removeChanop(client);
	this->removeChancreator(client);
}

void Channel::addChanop(Client *client)
{
	if (!this->isChanop(client))
		this->_chanops.push_back(client);
}

void Channel::removeChanop(Client *client)
{
	std::vector<Client *>::iterator it = this->_chanops.begin();
	if (this->isChanop(client))
	{
		while (it != this->_chanops.end())
		{
			if (*it == client)
			{
				this->_chanops.erase(it);
				break ;
			}
			it++;
		}
	}
}

void Channel::addChancreator(Client *client)
{
	if (!this->isChancreator(client))
		this->_chancreators.push_back(client);
}

void Channel::removeChancreator(Client *client)
{
	std::vector<Client *>::iterator it = this->_chancreators.begin();
	if (this->isChancreator(client))
	{
		while (it != this->_chancreators.end())
		{
			if (*it == client)
			{
				this->_chancreators.erase(it);
				break ;
			}
			it++;
		}
	}
}

void Channel::sendToAll(std::string message)
{
	std::vector<Client *>::iterator it = this->_members.begin();
	while (it != this->_members.end())
	{
		(*it)->send(message);
		it++;
	}
}

void Channel::sendToAllButOne(std::string message, Client *client)
{
	std::vector<Client *>::iterator it = this->_members.begin();
	while (it != this->_members.end())
	{
		if (*it != client)
			(*it)->send(message);
		it++;
	}
}

std::string	Channel::getModes() const
{
	std::string			modes;
	std::string			limit;
	std::stringstream	ss;
	ss << this->getLimit(); ss >> limit;

	modes = this->getInviteFlag() ? "i" : "";
	modes += this->getLimitFlag() ? "l" : "";
	modes += this->getPasswordFlag() ? "k" : "";
	modes += this->getTopicFlag() ? "t" : "";
	modes += this->getLimitFlag() ? " " + limit : "";
	modes += this->getPasswordFlag() ? " " + this->getPassword() : "";
	if (modes.size() > 0)
		modes.insert(0, "+");
	return (modes);
}

// Static functions

Channel * checkChannelExist(std::string channelName, std::vector<Channel *> channels)
{
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if ((*it)->getName() == channelName)
			return (*it);
	}
	return (NULL);
}
