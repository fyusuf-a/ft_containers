/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty.pass.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaceres <ecaceres@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 19:45:09 by ecaceres          #+#    #+#             */
/*   Updated: 2022/04/25 13:50:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unit_map.hpp>
#include <map.hpp>

int
main(void)
{
	{
		typedef MAP<Aware<int>, Aware<double> > M;
		M m;

		ASSERT(m.empty());

		m.insert(M::value_type(1, 1.5));
		ASSERT(!m.empty());

		m.clear();
		ASSERT(m.empty());
	}

	ASSERT_AWARE_ZERO();
}
