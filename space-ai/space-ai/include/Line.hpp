#ifndef SF_LINE_HPP
#define SF_LINE_HPP

#include <SFML/Graphics.hpp>

class Line : public sf::Drawable
{
public:
	Line();
	Line(const sf::Vector2f& point1, const sf::Vector2f& point2);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	void setPoint1(sf::Vector2f point1);
	void setPoint2(sf::Vector2f point2);
	void setColor(sf::Color color);
	void setThickness(float thickness);

private:
	void rebuild();

	sf::Vertex m_vertices[4];
	float m_thickness;
	sf::Color m_color;
	sf::Vector2f m_point1;
	sf::Vector2f m_point2;
};

#endif
