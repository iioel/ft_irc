/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:16:09 by yoel              #+#    #+#             */
/*   Updated: 2023/07/30 19:11:55 by lulutalu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <ctime>
# include "Client.hpp"

class Channel
{
	private:
		std::string 				_name;
		std::string					_topic;
		Client*						_whoTopic;
		time_t						_whenTopic;
		std::string					_password;
		time_t						_creationTime;
		int							_limit;
		bool						_inviteFlag;
		bool						_topicFlag;
		bool						_passwordFlag;
		bool						_limitFlag;
		std::vector<Client *>		_invited;
		std::vector<Client *> 		_members;
		std::vector<Client *>		_chanops;

	public:
		Channel(std::string name);
		Channel(std::string name, std::string password);
		~Channel();
		Channel(Channel const & src);
		Channel & operator=(Channel const & src);
		bool	operator==(Channel const & src) const;

		// Getters
		std::string 			getName() const;
		std::string				getPassword() const;
		std::string				getTopic() const;
		Client*					whoSetTopic();
		time_t					whenSetTopic() const;
		std::string				getStrWhenTopic() const;
		std::string				getStrCreationTime() const;
		time_t					getCreationTime() const;
		int						getLimit() const;
		std::vector<Client *> 	getInvited() const;
		std::vector<Client *> 	getMembers() const;
		std::vector<Client *> 	getChanops() const;

		bool 					isInvited(Client *client) const;

		Client*					getClientFromNickname(std::string nickname);

		bool 					isMember(Client *client) const;
		bool 					isChanop(Client *client) const;

		bool 					getInviteFlag() const;
		bool 					getTopicFlag() const;
		bool 					getPasswordFlag() const;
		bool 					getLimitFlag() const;

		// Setters
		void setName(std::string name);
		void setPassword(std::string password);
		void setTopic(std::string topic);
		void setWhoTopic(Client* client);
		void setWhenTopic(time_t when);
		void setLimit(int limit);
		void setInvited(std::vector<Client *> invited);
		void setMembers(std::vector<Client *> members);
		void setChanops(std::vector<Client *> chanops);
		void setInviteFlag(bool inviteFlag);
		void setTopicFlag(bool topicFlag);
		void setPasswordFlag(bool passwordFlag);
		void setLimitFlag(bool limitFlag);

		void addInvited(Client *client);
		void removeInvited(Client *client);

		void addMember(Client *client);
		void removeMember(Client *client);

		void addChanop(Client *chanop);
		void removeChanop(Client *chanop);

		void sendToAll(std::string message);
		void sendToAllButOne(std::string message, Client *client);
		std::string	getModes() const;

};


#endif
