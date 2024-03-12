bool RCCollision(sf::RectangleShape Rectangle, sf::CircleShape Circle)
{
    if (abs(Rectangle.getPosition().x - Circle.getPosition().x) < abs(Circle.getRadius() + Rectangle.getSize().x / 2) && abs(Rectangle.getPosition().y - Circle.getPosition().y) < abs(Circle.getRadius() + Rectangle.getSize().y / 2)) {
        return true;
    }
    else
    {
        return false;
    }
}
