#include "objects.h"

// Constructor por defecto de la clase Objects
Objects::Objects()
{
    // El constructor no realiza ninguna operación
}

// Función para vincular objetos con 3 normales, 2 coordenadas de textura
void Objects::link5(GLsizeiptr size, GLfloat* vertices)
{
    // Generar un VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // Generar un VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    // Vincular el VBO al array de vértices y crear un buffer del tamaño de los vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // Vincular el VAO
    glBindVertexArray(VAO);
    // Vincular los punteros de atributos del VBO al VAO
    // Nuestros vértices tienen 3 floats por posición y 2 por coordenada de textura, totalizando 5 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(VAO); // Asegurar que el VAO está vinculado
}

// Función para vincular objetos con 3 normales, 3 posiciones, 2 coordenadas de textura
void Objects::link(GLsizeiptr size, GLfloat* vertices)
{
    // Generar un VAO
    glGenVertexArrays(1, &VAO);
    // Generar un VBO
    glGenBuffers(1, &VBO);
    // Vincular el VBO al array de vértices y crear un buffer del tamaño de los vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // Vincular el VAO
    glBindVertexArray(VAO);
    // Vincular los punteros de atributos del VBO al VAO
    // Nuestros vértices tienen 3 floats por posición, 3 floats por normal, y 2 por coordenada de textura, totalizando 8 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(VAO); // Asegurar que el VAO está vinculado
}

// Función para vincular skybox u objetos que usan solo 3 posiciones
void Objects::skybox(GLsizeiptr size, GLfloat* vertices)
{
    // Generar un VAO
    glGenVertexArrays(1, &VAO);
    // Generar un VBO
    glGenBuffers(1, &VBO);
    // Vincular el VAO
    glBindVertexArray(VAO);
    // Vincular el VBO al array de vértices y crear un buffer del tamaño de los vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // Vincular los punteros de atributos del VBO al VAO
    // Nuestros vértices tienen 3 floats por posición
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(VAO); // Asegurar que el VAO está vinculado
}

// Función para dibujar el objeto
void Objects::bind()
{
    // Dibujar el objeto con GL_TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Desvincular el VAO
    glBindVertexArray(0);
}

// Función para dibujar skybox
void Objects::bindSkybox()
{
    // Dibujar el skybox con GL_TRIANGLES
    glDrawArrays(GL_TRIANGLES, 0, 72);
    // Desvincular el VAO
    glBindVertexArray(0);
}

// Función para limpiar VAO y VBOs
void Objects::clear()
{
    // Eliminar el VAO y el VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
