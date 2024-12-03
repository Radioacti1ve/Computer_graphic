#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

//     g++ -o lab5 lab5.cpp -lsfml-graphics -lsfml-window -lsfml-system
// Структура для представления вектора и основных операций с ним
struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator + (const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator - (const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator * (float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator / (float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const {
        float len = length();
        return *this / len;
    }
};

// Структура для представления луча
struct Ray {
    Vec3 origin, direction;
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction.normalize()) {}
};

// Примитивы: Сферы и Плоскости
struct Sphere {
    Vec3 center;
    float radius;
    Sphere(const Vec3& center, float radius) : center(center), radius(radius) {}

    bool intersect(const Ray& ray, float& t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;

        if (discriminant > 0) {
            t = (-b - sqrt(discriminant)) / (2.0f * a);
            return true;
        }
        return false;
    }

    Vec3 get_normal(const Vec3& point) const {
        return (point - center).normalize();
    }
};

struct Plane {
    Vec3 normal;
    float d;  // Расстояние от начала координат
    Vec3 color; // Цвет плоскости
    Plane(const Vec3& normal, float d, const Vec3& color) : normal(normal), d(d), color(color) {}

    bool intersect(const Ray& ray, float& t) const {
        float denom = normal.dot(ray.direction);
        if (fabs(denom) > 1e-6) {  // Погрешность для избежания деления на ноль
            t = -(ray.origin.dot(normal) + d) / denom;
            return (t >= 0);  // Пересечение только в положительном направлении
        }
        return false;
    }
};

// Моделирование объёмного рассеяния
float compute_volume_effect(const Vec3& point, const Vec3& lightDir, float scattering, float absorption) {
    // Расстояние до источника света
    float distance = point.length();
    
    // Моделируем поглощение и рассеяние света через экспоненциальные функции
    float attenuation = exp(-absorption * distance);
    float scatter = exp(-scattering * distance);

    // Рассчитываем интенсивность света в зависимости от угла
    float angleEffect = std::max(0.0f, lightDir.dot(point.normalize()));
    
    // Учитываем рассеяние и поглощение
    return attenuation * scatter * angleEffect;
}

// Моделирование эффекта дыма
float compute_smoke_effect(const Vec3& point, const Vec3& lightDir, float scattering, float absorption, float smokeDensity) {
    // Рассчитываем расстояние до точки от начала координат
    float distance = point.length();
    
    // Моделируем поглощение и рассеяние через экспоненциальные функции
    float attenuation = exp(-absorption * distance);
    float scatter = exp(-scattering * distance);
    
    // Учитываем плотность дыма (поглощение и рассеяние света)
    float smokeFactor = exp(-smokeDensity * distance);

    // Интегрируем все эффекты
    return attenuation * scatter * smokeFactor;
}


// Моделируем перетекание дыма на плоскость
float compute_smoke_density_on_plane(const Vec3& point, const Plane& plane, float smokeDensity) {
    // Если точка находится ниже плоскости, увеличиваем плотность дыма
    if (point.y < plane.d) {
        return smokeDensity * (1.0f + (plane.d - point.y) * 0.1f);
    }
    return smokeDensity;
}


Vec3 calculate_lighting_with_smoke(const Vec3& hitPoint, const Vec3& normal, const Vec3& lightPos, const Vec3& lightColor, float scattering, float absorption, float smokeDensity) {
    // Направление от точки пересечения к источнику света
    Vec3 lightDir = (lightPos - hitPoint).normalize();

    // Стандартное освещение (рассеяние по нормали)
    float intensity = std::max(0.0f, normal.dot(lightDir));

    // Моделируем объёмное рассеяние и поглощение света через дым
    float volumeEffect = compute_volume_effect(hitPoint, lightDir, scattering, absorption);

    // Моделируем эффект дыма
    float smokeEffect = compute_smoke_effect(hitPoint, lightDir, scattering, absorption, smokeDensity);

    // Интегрируем объёмное рассеяние с базовой освещённостью и эффектом дыма
    return lightColor * intensity * (1.0f + volumeEffect + smokeEffect);
}


// Основная трассировка лучей
Vec3 trace_with_smoke(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Vec3& lightPos, float scattering, float absorption, float smokeDensity) {
    float t_min = 1e30f;
    Vec3 color(0, 0, 0); // Черный цвет, если нет пересечений

    // Пересечения с сферами
    for (const Sphere& sphere : spheres) {
        float t;
        if (sphere.intersect(ray, t)) {
            if (t < t_min) {
                t_min = t;
                Vec3 hitPoint = ray.origin + ray.direction * t;
                Vec3 normal = sphere.get_normal(hitPoint);

                // Вычисляем плотность дыма с учётом перетекания на плоскость
                for (const Plane& plane : planes) {
                    float smokeDensityOnPlane = compute_smoke_density_on_plane(hitPoint, plane, smokeDensity);
                    color = calculate_lighting_with_smoke(hitPoint, normal, lightPos, Vec3(1.0f, 1.0f, 0.5f), scattering, absorption, smokeDensityOnPlane);
                }
            }
        }
    }

    // Пересечения с плоскостями
    for (const Plane& plane : planes) {
        float t;
        if (plane.intersect(ray, t)) {
            if (t < t_min) {
                t_min = t;
                Vec3 hitPoint = ray.origin + ray.direction * t;
                Vec3 normal = plane.normal;

                // Вычисляем плотность дыма с учётом перетекания на плоскость
                float smokeDensityOnPlane = compute_smoke_density_on_plane(hitPoint, plane, smokeDensity);
                color = calculate_lighting_with_smoke(hitPoint, normal, lightPos, plane.color, scattering, absorption, smokeDensityOnPlane);
            }
        }
    }

    return color;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ray Tracing with Smoke Effect");

    // Объекты сцены
    std::vector<Sphere> spheres = {
        Sphere(Vec3(0, 0, -5), 1.0f),
        Sphere(Vec3(2, 0, -5), 1.0f)
    };

    std::vector<Plane> planes = {
        Plane(Vec3(0, 1, 0), -1.0f, Vec3(0.7f, 0.7f, 0.7f)),
        Plane(Vec3(0, -1, 0), -2.0f, Vec3(0.5f, 0.5f, 0.5f))
    };

    Vec3 lightPos(0, 5, -5);
    sf::Clock clock;

    // Параметры дыма
    float smokeDensity = 0.02f;
    float scattering = 0.1f;
    float absorption = 0.05f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                // Управление плотностью дыма с помощью стрелочек
                if (event.key.code == sf::Keyboard::Up) {
                    smokeDensity += 0.1f;  // Увеличиваем плотность дыма
                }
                if (event.key.code == sf::Keyboard::Down) {
                    smokeDensity -= 0.1f;  // Уменьшаем плотность дыма
                }
            }
        }

        window.clear();
        Vec3 cameraPos(0, 0, 0);
        float fov = 90.0f;
        float aspect = 800.0f / 600.0f;
        float scale = tan(fov * 0.5f * 3.14159f / 180.0f);

        for (int y = 0; y < 600; ++y) {
            for (int x = 0; x < 800; ++x) {
                float pixelX = (2 * (x + 0.5f) / 800.0f - 1) * aspect * scale;
                float pixelY = (1 - 2 * (y + 0.5f) / 600.0f) * scale;

                Vec3 direction(pixelX, pixelY, -1);
                Ray ray(cameraPos, direction);

                Vec3 color = trace_with_smoke(ray, spheres, planes, lightPos, scattering, absorption, smokeDensity);

                sf::Color pixelColor(
                    std::min(255, static_cast<int>(color.x * 255)),
                    std::min(255, static_cast<int>(color.y * 255)),
                    std::min(255, static_cast<int>(color.z * 255))
                );

                sf::Vertex pixel(sf::Vector2f(x, y), pixelColor);
                window.draw(&pixel, 1, sf::Points);
            }
        }

        window.display();
    }

    return 0;
}
