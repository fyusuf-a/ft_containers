/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_map.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 18:03:51 by ecaceres          #+#    #+#             */
/*   Updated: 2022/04/25 13:53:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIT_MAP_HPP_
# define UNIT_MAP_HPP_

# include "test_macros.hpp"
# include "test_containers.hpp"
# include "support_std.hpp"

# if TEST_USE_STD
#  include <map>
#  define MAP std::map
# else
#  include <map.hpp>
#  define MAP ft::map
# endif

#endif /* UNIT_MAP_HPP_ */
