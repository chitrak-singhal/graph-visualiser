#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <bits/stdc++.h>

class TextBox
{
private:
    sf::RectangleShape box;
    sf::RectangleShape cursor;
    sf::Text text;
    std::string input;

public:
    bool isSelected;

    TextBox(float x, float y, float width, float height, const sf::Font &font)
        : isSelected(false)
    {
        box.setPosition(x, y);
        box.setSize(sf::Vector2f(width, height));
        box.setFillColor(sf::Color::White);

        cursor.setSize(sf::Vector2f(2, height - 10));
        cursor.setFillColor(sf::Color::Black);
        cursor.setPosition(x + 5, y + 5);

        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(14);
        text.setPosition(x + 5, y + 5);
    }

    void render(sf::RenderWindow &window)
    {
        window.draw(box);
        window.draw(text);
        if (isSelected)
        {
            window.draw(cursor);
        }
    }

    bool contains(float x, float y)
    {
        return box.getGlobalBounds().contains(x, y);
    }

    void setSelected(bool selected)
    {
        isSelected = selected;
        if (!isSelected)
            cursor.setFillColor(sf::Color::Transparent);
        else
            cursor.setFillColor(sf::Color::Black);
    }

    std::string getInput()
    {
        return input;
    }

    void clear()
    {
        input.clear();
        text.setString(input);
        resetCursor();
    }

    void resetCursor()
    {
        cursor.setPosition(box.getPosition().x + text.getGlobalBounds().width + 5, box.getPosition().y + 5);
    }

    void handleInput(sf::Event &event)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (isSelected)
            {
                if (event.text.unicode == 8)
                {
                    if (!input.empty())
                    {
                        input.pop_back();
                    }
                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126)
                {
                    input += static_cast<char>(event.text.unicode);
                }
                text.setString(input);
                resetCursor();
            }
        }
    }
};
class Label
{
public:
    sf::Text text;
    Label(int charsize, float x, float y, sf::Font &font)
    {
        text.setFont(font);
        text.setCharacterSize(charsize);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
    }
};
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
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);
        label.setPosition((startX + endX) / 2 + 0.1, (startY + endY) / 2 + 0.1);
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
        // std::cout << "togglecalled\n";
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
class Array
{
private:
    std::vector<sf::RectangleShape> blocks;
    std::vector<sf::Text> texts;
    const float blockWidth = 50.0f;
    const float blockHeight = 50.0f;
    const float spacing = 10.0f;
    const float x = 50.0f;
    const float y = 50.0f;

public:
    Array(std::vector<int> &arr, const sf::Font &font)
    {
        int si = arr.size();
        for (int i = 0; i < si; i++)
        {
            sf::RectangleShape block(sf::Vector2f(blockWidth, blockHeight));
            block.setFillColor(sf::Color::White);
            block.setOutlineThickness(1.0f);
            block.setOutlineColor(sf::Color::Black);
            block.setPosition(x + i * (blockWidth + spacing), y); // Position the block in a row

            sf::Text text;
            text.setFont(font);
            if (arr[i] == INT_MAX)
                text.setString("MAX");
            else
                text.setString(std::to_string(arr[i]));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);

            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            text.setPosition(block.getPosition().x + blockWidth / 2.0f, block.getPosition().y + blockHeight / 2.0f);

            blocks.push_back(block);
            texts.push_back(text);
        }
    }

    void render(sf::RenderWindow &window)
    {
        for (size_t i = 0; i < blocks.size(); i++)
        {
            window.draw(blocks[i]);
            window.draw(texts[i]);
        }
    }
};
int main()
{
    int window_width = 1440, window_height = 900;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Graph Visualizer");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return EXIT_FAILURE;
    }

    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<int> selectedNodes;

    int panel_width = 300;
    int panel_start = window_width - panel_width;
    sf::RectangleShape panel(sf::Vector2f(panel_width, window_height));
    panel.setFillColor(sf::Color(50, 50, 50));
    panel.setPosition(panel_start, 0);

    TextBox inputBoxWeight(panel_start + 10, 80, 160, 30, font);

    bool waitingForWeight = false;

    Label weightLabel(18, panel_start + 10, 20, font);

    std::vector<int> dist(5, INT_MAX);
    Array distArray(dist, font);

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

                    if (x < panel_start)
                    {
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
                        // std::cout << selectedNodes.size() << "\n";
                        if (selectedNodes.size() == 2)
                        {
                            waitingForWeight = true;
                            inputBoxWeight.setSelected(true);
                        }
                    }
                    else
                    {
                        if (inputBoxWeight.contains(x, y))
                            inputBoxWeight.setSelected(true);
                        else
                            inputBoxWeight.setSelected(false);
                    }
                }
            }
            if (inputBoxWeight.isSelected)
                inputBoxWeight.handleInput(event);

            if (!waitingForWeight)
                weightLabel.text.setString("Select two nodes to create an\nedge.");
            else
            {
                int startNodeIndex = selectedNodes[0];
                int endNodeIndex = selectedNodes[1];

                std::string x = "Enter weight for the edge\nbetween node " + std::to_string(startNodeIndex + 1) +
                                " and node " + std::to_string(endNodeIndex + 1) + ": ";
                weightLabel.text.setString(x);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                std::stringstream ss(inputBoxWeight.getInput());
                if (waitingForWeight)
                {
                    int weight;
                    ss >> weight;
                    int startNodeIndex = selectedNodes[0];
                    int endNodeIndex = selectedNodes[1];

                    edges.emplace_back(startNodeIndex, endNodeIndex, weight, font,
                                       nodes[startNodeIndex].x, nodes[startNodeIndex].y,
                                       nodes[endNodeIndex].x, nodes[endNodeIndex].y);

                    nodes[startNodeIndex].toggleColor();
                    nodes[endNodeIndex].toggleColor();
                    selectedNodes.clear();

                    waitingForWeight = false;
                    inputBoxWeight.clear();
                    inputBoxWeight.setSelected(false);
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
        window.draw(panel);
        window.draw(weightLabel.text);
        inputBoxWeight.render(window);
        distArray.render(window);
        window.display();
    }

    return 0;
}
