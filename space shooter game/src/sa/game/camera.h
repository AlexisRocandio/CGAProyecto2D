#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Enumeración para movimientos de la cámara
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Valores por defecto para la cámara
const float YAW = -90.0f;          // Ángulo de giro horizontal
const float PITCH = 0.0f;          // Ángulo de giro vertical
const float SPEED = 2.5f;          // Velocidad de movimiento
const float SENSITIVITY = 0.1f;    // Sensibilidad del ratón
const float ZOOM = 45.0f;          // Ángulo de zoom

// Clase para la cámara
class Camera
{
public:
    // Atributos de la cámara
    glm::vec3 Position;        // Posición de la cámara
    glm::vec3 Front;           // Dirección hacia la que mira la cámara
    glm::vec3 Up;              // Vector hacia arriba
    glm::vec3 Right;           // Vector hacia la derecha
    glm::vec3 WorldUp;         // Vector hacia arriba del mundo
    float Yaw;                 // Ángulo horizontal
    float Pitch;               // Ángulo vertical
    float MovementSpeed;       // Velocidad de movimiento
    float MouseSensitivity;    // Sensibilidad del ratón
    float Zoom;                // Nivel de zoom

    // Constructor con parámetros por defecto
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors(); // Actualiza los vectores de la cámara
    }

    // Constructor con parámetros específicos
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors(); // Actualiza los vectores de la cámara
    }

    // Devuelve la matriz de vista basada en la posición y la dirección de la cámara
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Procesa la entrada del teclado
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Procesa el movimiento del ratón
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Asegura que el ángulo de pitch no exceda ciertos límites
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors(); // Actualiza los vectores de la cámara basados en los nuevos ángulos
    }

    // Procesa el desplazamiento del ratón para el zoom
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // Actualiza los vectores de la cámara para reflejar los cambios en yaw y pitch
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp)); // Calcula el vector a la derecha
        Up = glm::normalize(glm::cross(Right, Front));      // Calcula el vector hacia arriba
    }
};

#endif
