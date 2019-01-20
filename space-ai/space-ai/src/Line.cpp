#include "Line.hpp"

Line::Line()
	:
	m_thickness(1.f)
{
}

Line::Line(const sf::Vector2f& point1, const sf::Vector2f& point2)
	:
	m_thickness(5.f),
	m_color(sf::Color::Yellow),
	m_point1(point1),
	m_point2(point2)
{
	rebuild();
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices, 4, sf::Quads);
}

void Line::setPoint1(sf::Vector2f point1)
{
	m_point1 = point1;
	rebuild();
}

void Line::setPoint2(sf::Vector2f point2)
{
	m_point2 = point2;
	rebuild();
}

void Line::setColor(sf::Color color)
{
	m_color = color;
	rebuild();
}

void Line::setThickness(float thickness)
{
	m_thickness = thickness;
	rebuild();
}

void Line::rebuild()
{
	sf::Vector2f direction = m_point2 - m_point1;
	sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
	sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

	sf::Vector2f offset = (m_thickness / 2.f)*unitPerpendicular;

	m_vertices[0].position = m_point1 + offset;
	m_vertices[1].position = m_point2 + offset;
	m_vertices[2].position = m_point2 - offset;
	m_vertices[3].position = m_point1 - offset;

	for (int i = 0; i<4; ++i)
		m_vertices[i].color = m_color;
}
