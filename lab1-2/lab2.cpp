#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

float rotationSpeed = 1.0f;
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
}

void drawCube() {
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glColor3f(0.6f, 0.8f, 0.6f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.7f, 0.8f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glColor3f(1.0f, 1.0f, 0.6f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(0.8f, 0.6f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glColor3f(1.0f, 0.7f, 0.5f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Cube Rotation with OpenGL", sf::Style::Close | sf::Style::Resize);
    initOpenGL();
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float deltaSeconds = deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) rotationSpeed += 2.0f;
                if (event.key.code == sf::Keyboard::Down) rotationSpeed -= 2.0f;
                if (rotationSpeed < 0.0f) rotationSpeed = 0.0f;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        angleX += rotationSpeed * deltaSeconds;
        angleY += rotationSpeed * deltaSeconds;
        angleZ += rotationSpeed * deltaSeconds;

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);

        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

        drawCube();

        window.display();
    }

    return 0;
}

// #include <SFML/Graphics.hpp>
// #include <SFML/OpenGL.hpp>
// #include <cmath>
// #include <vector>

// // Структура для хранения координат вершины
// struct Point {
//     float x, y;
// };

// // Функция для создания многоугольника
// std::vector<Point> createPolygon(int sides, float radius) {
//     std::vector<Point> vertices;
//     float angleStep = 2 * M_PI / sides;
//     for (int i = 0; i < sides; ++i) {
//         float angle = i * angleStep;
//         // Приводим результат к типу float
//         vertices.push_back({float(radius * cos(angle)), float(radius * sin(angle))});
//     }
//     return vertices;
// }


// // Функция для отрисовки многоугольника с применением трансформаций
// void drawPolygon(sf::RenderWindow& window, const std::vector<Point>& vertices, sf::Vector2f position, float scale, float rotation) {
//     sf::VertexArray polygon(sf::TriangleFan, vertices.size());

//     for (size_t i = 0; i < vertices.size(); ++i) {
//         float x = vertices[i].x * scale;
//         float y = vertices[i].y * scale;

//         float rotatedX = x * cos(rotation) - y * sin(rotation);
//         float rotatedY = x * sin(rotation) + y * cos(rotation);

//         polygon[i].position = sf::Vector2f(rotatedX, rotatedY) + position;
//         polygon[i].color = sf::Color::Green;
//     }

//     window.draw(polygon);
// }

// // Функция для обновления позиции по замкнутой траектории
// void updatePosition(float& time, sf::Vector2f& position, float radius) {
//     time += 0.01f; // скорость движения
//     position.x = 400 + radius * cos(time); // центрируем по (400, 300)
//     position.y = 300 + radius * sin(time);
// }

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "2D Transformations", sf::Style::Close);
//     window.setFramerateLimit(60);

//     int sides = 5;               // Число сторон многоугольника
//     float polygonRadius = 50.0f;  // Радиус многоугольника
//     auto vertices = createPolygon(sides, polygonRadius);

//     sf::Vector2f position(400, 300); // Начальная позиция
//     float scale = 1.0f;
//     float rotation = 0.0f;
//     float time = 0.0f;

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         // Управление трансформациями
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= 1.0f;
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += 1.0f;
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= 1.0f;
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += 1.0f;

//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) rotation -= 0.01f;
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) rotation += 0.01f;

//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) scale += 0.01f;
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) scale -= 0.01f;

//         // Обновление позиции для замкнутой траектории
//         updatePosition(time, position, 100.0f);

//         window.clear(sf::Color::Black);
//         drawPolygon(window, vertices, position, scale, rotation);
//         window.display();
//     }

//     return 0;
// }
