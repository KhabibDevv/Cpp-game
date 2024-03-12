bool RCCollision(sf::RectangleShape Rectangle, sf::CircleShape Circle) // function that returns true if a Rectangle and a sphere are coliding, false if not
{
    if (abs(Rectangle.getPosition().x - Circle.getPosition().x) < abs(Circle.getRadius() + Rectangle.getSize().x / 2) && abs(Rectangle.getPosition().y - Circle.getPosition().y) < abs(Circle.getRadius() + Rectangle.getSize().y / 2)) {
        return true;
    }
    else
    {
        return false;
    }
}
