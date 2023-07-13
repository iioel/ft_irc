/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoel <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 11:24:18 by yoel              #+#    #+#             */
/*   Updated: 2023/07/14 13:11:20 by yoel             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

class Command
{
	public:
		Command(std::string command);
		~Command();
		Command(const Command &src);
		Command &operator=(const Command &src);
	private:
		Command();
		int _parse();

		std::string 				_command;
		std::string 				_prefix;
		std::vector<std::string> 	_params;
};

#endif
