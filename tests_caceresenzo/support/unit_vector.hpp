/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_vector.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 18:03:51 by ecaceres          #+#    #+#             */
/*   Updated: 2022/03/31 19:21:47 by fyusuf-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIT_VECTOR_HPP_
# define UNIT_VECTOR_HPP_

# include "test_macros.hpp"
# include "test_containers.hpp"
# include "support_std.hpp"

# if TEST_USE_STD
#  include <vector>
#  define VECTOR std::vector
# else
#  include <vector.hpp>
#  define VECTOR ft::vector
# endif

#endif /* UNIT_VECTOR_HPP_ */
