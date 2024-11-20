//  g++ lab3.cpp -o lab3 -lGL -lGLU -lsfml-window -lsfml-system

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <iostream>

// Переменные для управления вращением
float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
bool rotateAroundCamera = true; // Флаг для переключения режима вращения

float cameraPosX = 0.0f;
float cameraPosY = 0.0f;
float cameraPosZ = 5.0f; 

// Функция для инициализации сцены
void initializeScene() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Функция для отрисовки сцены
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Перемещение камеры

    if (rotateAroundCamera) {
        // Вращение относительно камеры
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);
        gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    } else {
        // Вращение относительно центра объекта
        gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glTranslatef(0.0f, 0.0f, -2.0f);
        glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);

    }

    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f); // Красная грань
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);

        glColor3f(0.0f, 1.0f, 0.0f); // Зеленая грань
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        glColor3f(0.0f, 0.0f, 1.0f); // Синяя грань
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        glColor3f(1.0f, 1.0f, 0.0f); // Желтая грань
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        glColor3f(1.0f, 0.0f, 1.0f); // Фиолетовая грань
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        glColor3f(0.0f, 1.0f, 1.0f); // Голубая грань
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Transformations", sf::Style::Default, sf::ContextSettings(24));
    window.setActive(true);

    initializeScene();

    bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                running = false;
            }
            // Обработка событий для управления вращением
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    rotationAngleY -= 5.0f;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    rotationAngleY += 5.0f;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    rotationAngleX -= 5.0f;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    rotationAngleX += 5.0f;
                }
                if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    rotateAroundCamera = !rotateAroundCamera;
                    if (rotateAroundCamera) {
                        // Задайте позицию камеры для вращения вокруг объекта
                        cameraPosX = 5.0f; // Или любое другое значение для позиции камеры
                        cameraPosY = 5.0f;
                        cameraPosZ = 5.0f;
                    } else {
                        // Верните камеру в стандартное положение
                        cameraPosX = 0.0f;
                        cameraPosY = 0.0f;
                        cameraPosZ = 5.0f;
                    }
                    std::cout << "Space key pressed. rotateAroundCamera: " << rotateAroundCamera << std::endl;
                }
}


            }
        }

        drawScene();
        window.display();
    }

    return 0;
}
