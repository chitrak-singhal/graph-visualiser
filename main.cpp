#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <bits/stdc++.h>
struct Edge
{
    int startNodeIndex;
    int endNodeIndex;
    int weight;
    sf::Text label;
    sf::VertexArray line;

    Edge(int start, int end, float w, const sf::Font &font, float startX, float startY, float endX, float endY)
        : startNodeIndex(start), endNodeIndex(end), weight(w), line(sf::Lines, 2)
    {

        line[0].position = sf::Vector2f(startX, startY);
        line[0].color = sf::Color::White;
        line[1].position = sf::Vector2f(endX, endY);
        line[1].color = sf::Color::White;

        label.setFont(font);
        label.setString(std::to_string(weight));
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::White);
        label.setPosition((startX + endX) / 2, (startY + endY) / 2);
    }
};
struct Node
{
    sf::CircleShape shape;
    sf::Text label;
    float x, y;
    bool selected;
    Node(float x, float y, const sf::Font &font, const std::string &labelText) : x(x), y(y), selected(false)
    {
        shape.setRadius(25.0f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(x - shape.getRadius(), y - shape.getRadius());

        label.setFont(font);
        label.setString(labelText);
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::Black);
        label.setPosition(x - shape.getRadius() / 2, y - shape.getRadius() / 2);
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        label.setPosition(x, y);
    }
    bool contains(float px, float py)
    {
        float dx = px - (x);
        float dy = py - (y);
        return dx * dx + dy * dy <= shape.getRadius() * shape.getRadius();
    }

    void toggleColor()
    {
        std::cout << "togglecalled\n";
        selected = !selected;
        if (selected)
        {
            shape.setFillColor(sf::Color::Green);
        }
        else
        {
            shape.setFillColor(sf::Color::Yellow);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualizer");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }

    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<int> selectedNodes;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    bool nodeClicked = false;
                    for (size_t i = 0; i < nodes.size(); ++i)
                    {
                        if (nodes[i].contains(x, y))
                        {
                            nodes[i].toggleColor();
                            if (nodes[i].selected)
                            {
                                selectedNodes.push_back(i);
                            }
                            else
                            {
                                selectedNodes.erase(std::remove(selectedNodes.begin(), selectedNodes.end(), i), selectedNodes.end());
                            }
                            nodeClicked = true;
                            break;
                        }
                    }

                    if (!nodeClicked)
                    {
                        nodes.emplace_back(x, y, font, std::to_string(nodes.size() + 1));
                    }
                    std::cout << selectedNodes.size() << "\n";
                    if (selectedNodes.size() == 2)
                    {

                        int startNodeIndex = selectedNodes[0];
                        int endNodeIndex = selectedNodes[1];

                        std::cout << "Enter weight for the edge between node " << startNodeIndex + 1
                                  << " and node " << endNodeIndex + 1 << ": ";
                        int weight;
                        std::cin >> weight;

                        edges.emplace_back(startNodeIndex, endNodeIndex, weight, font,
                                           nodes[startNodeIndex].x, nodes[startNodeIndex].y,
                                           nodes[endNodeIndex].x, nodes[endNodeIndex].y);

                        nodes[startNodeIndex].toggleColor();
                        nodes[endNodeIndex].toggleColor();
                        selectedNodes.clear();
                    }
                }
            }
        }

        window.clear();
        for (const auto &edge : edges)
        {
            window.draw(edge.line);
            window.draw(edge.label);
        }
        for (const auto &node : nodes)
        {
            window.draw(node.shape);
            window.draw(node.label);
        }
        window.display();
    }

    return 0;
}
