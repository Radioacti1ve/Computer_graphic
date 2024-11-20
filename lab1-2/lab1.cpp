#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
// g++ -o main main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lGL 
using namespace sf;

const int linesAmount = 150;
const int X_SIZE = 500;       
const int Y_SIZE = 500;       
const int X_POSITION = 700;   
const int Y_POSITION = 400;   

int main() {
    srand(time(NULL)); 
    RenderWindow window(VideoMode(1400, 800), L"Cohen-Sutherland", Style::Default); 

    RectangleShape renderingArea(Vector2f(X_SIZE, Y_SIZE));
    renderingArea.setFillColor(Color(25, 75, 75)); 
    renderingArea.setOrigin(Vector2f(X_SIZE / 2, Y_SIZE / 2)); 
    renderingArea.setPosition(X_POSITION, Y_POSITION); 
    renderingArea.setOutlineThickness(1); 
    renderingArea.setOutlineColor(Color(50, 150, 150)); 

    VertexArray lines(Lines, linesAmount);
    VertexArray startLines(Lines, linesAmount);
    VertexArray _lines(Lines); 
    char bitCodes[linesAmount]; 

    double Xmin = renderingArea.getPosition().x - X_SIZE / 2;
    double Ymin = renderingArea.getPosition().y - Y_SIZE / 2;
    double Xmax = renderingArea.getPosition().x + X_SIZE / 2;
    double Ymax = renderingArea.getPosition().y + Y_SIZE / 2;

    for (int i = 0; i < linesAmount; ++i) {
        lines[i].position = Vector2f(rand() % 1400, rand() % 800); 
        lines[i].color = Color(0, 100, 125); 
        startLines[i].position = lines[i].position;
        startLines[i].color = Color::Magenta; 
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close(); 
        }

        Xmin = renderingArea.getPosition().x - X_SIZE / 2;
        Ymin = renderingArea.getPosition().y - Y_SIZE / 2;
        Xmax = renderingArea.getPosition().x + X_SIZE / 2;
        Ymax = renderingArea.getPosition().y + Y_SIZE / 2;

        for (int i = 0; i < linesAmount; ++i) {
            bitCodes[i] = 0;
            if (lines[i].position.x >= Xmin) { bitCodes[i] &= ~(1u); }
            else { bitCodes[i] |= (1u); }
            if (lines[i].position.x <= Xmax) { bitCodes[i] &= ~(1u << 1); }
            else { bitCodes[i] |= (1u << 1); }
            if (lines[i].position.y >= Ymin) { bitCodes[i] &= ~(1u << 2); }
            else { bitCodes[i] |= (1u << 2); }
            if (lines[i].position.y <= Ymax) { bitCodes[i] &= ~(1u << 3); }
            else { bitCodes[i] |= (1u << 3); }
        }

        for (int i = 0; i < linesAmount - 1; i += 2) {
            if ((bitCodes[i] == 0) && (bitCodes[i + 1] == 0)) {
                lines[i].color = Color(255, 0, 0);
                lines[i + 1].color = Color(255, 0, 0);
                continue;
            } else if (bitCodes[i] & bitCodes[i + 1]) {
                continue;
            } else {
                char code_out;
                double x = 0, y = 0;
                double x1 = lines[i].position.x;
                double x2 = lines[i + 1].position.x;
                double y1 = lines[i].position.y;
                double y2 = lines[i + 1].position.y;

                if (bitCodes[i] != 0) { code_out = bitCodes[i]; }
                else { code_out = bitCodes[i + 1]; }

                if (code_out & (1u << 3)) { 
                    if (code_out == bitCodes[i]) {
                        y = Ymax;
                        x = x1 + (x2 - x1) * (Ymax - y1) / (y2 - y1);
                    } else {
                        y = Ymax;
                        x = x2 + (x1 - x2) * (Ymax - y2) / (y1 - y2);
                    }
                } else if (code_out & (1u << 2)) { 
                    if (code_out == bitCodes[i]) {
                        y = Ymin;
                        x = x1 + (x2 - x1) * (Ymin - y1) / (y2 - y1);
                    } else {
                        y = Ymin;
                        x = x2 + (x1 - x2) * (Ymin - y2) / (y1 - y2);
                    }
                } else if (code_out & (1u << 1)) { 
                    if (code_out == bitCodes[i]) {
                        x = Xmax;
                        y = y1 + (y2 - y1) * (Xmax - x1) / (x2 - x1);
                    } else {
                        x = Xmax;
                        y = y2 + (y1 - y2) * (Xmax - x2) / (x1 - x2);
                    }
                } else if (code_out & (1u)) { 
                    if (code_out == bitCodes[i]) {
                        x = Xmin;
                        y = y1 + (y2 - y1) * (Xmin - x1) / (x2 - x1);
                    } else {
                        x = Xmin;
                        y = y2 + (y1 - y2) * (Xmin - x2) / (x1 - x2);
                    }
                }

                if (code_out == bitCodes[i]) {
                    Vertex point1(Vector2f(x1, y1), Color(0, 0, 255));
                    Vertex point2(Vector2f(x, y), Color(0, 0, 255));
                    _lines.append(point1);
                    _lines.append(point2);
                    lines[i].position.x = x;
                    lines[i].position.y = y;
                } else {
                    Vertex point1(Vector2f(x2, y2), Color(0, 0, 255));
                    Vertex point2(Vector2f(x, y), Color(0, 0, 255));
                    _lines.append(point1);
                    _lines.append(point2);
                    lines[i + 1].position.x = x;
                    lines[i + 1].position.y = y;
                }
            }
        }

        window.clear(Color::Black);
        window.draw(renderingArea); 
        window.draw(lines);         
        window.draw(_lines);        
        window.display();           
    }
    return 0;
}
