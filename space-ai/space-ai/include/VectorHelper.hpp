#ifndef VECTOR_HELPER_HPP
#define VECTOR_HELPER_HPP

#include <SFML/System/Vector2.hpp>

namespace vhelp
{
	template <class T> float length(const sf::Vector2<T>& vector)
	{
		float length = sqrt(vector.x * vector.x + vector.y * vector.y);
		return length;
	}

	template <class T> void normalize(sf::Vector2<T>* pVector)
	{
		(*pVector) /= length(*pVector);
	}
}

#endif