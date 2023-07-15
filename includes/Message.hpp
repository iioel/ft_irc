/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 16:52:28 by yoel              #+#    #+#             */
/*   Updated: 2023/07/15 17:02:34 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

class Message
{
	private:
		std::string _message;
		std::string _prefix;
		std::vector<std::string> _params;

		void _parse();
	
	public:
		Message();
		Message(std::string message);
		Message(Message const & src);
		~Message();
		Message & operator=(Message const & src);
	
		std::string getMessage() const;
		std::string getPrefix() const;
		std::vector<std::string> getParams() const;
	
		void setMessage(std::string message);
	
};

#endif
