#include "ball_object.h"

// Constructor por defecto
BallObject::BallObject() 
    : GameObject(), Radius(15.0f)  { }

// Constructor parametrizado
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius) { }

// Función para mover la bola
glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // Actualiza la posición de la bola basándose en la velocidad y el tiempo delta
    this->Position += this->Velocity * dt;

    // Devuelve la nueva posición
    return this->Position;
}

// Función para reiniciar la bola a una posición y velocidad específicas
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position; // Establece la nueva posición
    this->Velocity = velocity; // Establece la nueva velocidad
}
