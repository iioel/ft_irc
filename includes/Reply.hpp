/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycornamu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 17:33:53 by ycornamu          #+#    #+#             */
/*   Updated: 2023/07/20 16:02:25 by ycornamu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Reply_HPP
# define Reply_HPP

# define RPL_WELCOME "001"
# define RPL_YOURHOST "002"
# define RPL_CREATED "003"
# define RPL_MYINFO "004"
# define RPL_MOTDSTART "375"
# define RPL_MOTD "372"
# define RPL_ENDOFMOTD "376"
# define RPL_CHANNELMODEIS "324"
# define RPL_CREATIONTIME "329"
# define RPL_TOPIC "332"
# define RPL_NAMREPLY "353"
# define RPL_ENDOFNAMES "366"

# define ERR_NOSUCHNICK "401"
# define ERR_NOSUCHCHANNEL "403"
# define ERR_CANNOTSENDTOCHAN "404"
# define ERR_NOORIGIN "409"
# define ERR_NORECIPIENT "411"
# define ERR_NOTEXTTOSEND "412"
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKNAMEINUSE "433"
# define ERR_USERNOTINCHANNEL "441"
# define ERR_NOTONCHANNEL "442"
# define ERR_USERONCHANNEL "443"
# define ERR_NOTREGISTERED "451"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_ALREADYREGISTRED "462"
# define ERR_PASSWDMISMATCH "464"
# define ERR_CHANNELISFULL "471"
# define ERR_UNKNOWNMODE "472"
# define ERR_INVITEONLYCHAN "473"
# define ERR_BADCHANNELKEY "475"
# define ERR_BADCHANMASK "476"
# define ERR_CHANOPRIVSNEEDED "482"
# define ERR_USERSDONTMATCH "502"
# define ERR_INVALIDMODEPARAM "696"

#endif
