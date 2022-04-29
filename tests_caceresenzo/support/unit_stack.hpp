/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_stack.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 18:03:51 by ecaceres          #+#    #+#             */
/*   Updated: 2022/04/29 15:15:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIT_STACK_HPP_
# define UNIT_STACK_HPP_

# include "test_macros.hpp"
# include "test_containers.hpp"
# include "support_std.hpp"

# if TEST_USE_STD
#  include <stack>
#  define STACK std::stack
# else
#  include <stack.hpp>
#  define STACK ft::stack
# endif

#endif /* UNIT_STACK_HPP_ */
